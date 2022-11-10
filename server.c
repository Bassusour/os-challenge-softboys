#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <messages.h>
#include <server.h>
//#include "hashcache.h"
#include <reversehash.h>
#include <pthread.h>
#include "linkedlist.h"

#define SA struct sockaddr

Request new_request(int connfd)
{
    unsigned char in_buffer[PACKET_REQUEST_SIZE];
    Request req;
    uint64_t start = 0;
    uint64_t end = 0;
    uint8_t prio = 0;
    bzero(in_buffer, PACKET_REQUEST_SIZE);
    read(connfd, in_buffer, sizeof(in_buffer));
    memcpy(req.hash, &in_buffer[PACKET_REQUEST_HASH_OFFSET], 32);
    prio = in_buffer[PACKET_REQUEST_PRIO_OFFSET];

    for (uint64_t i = 0; i < 8; i++)
    {
        start = start | ((uint64_t)in_buffer[39 - i] << i * 8);
        end = end | ((uint64_t)in_buffer[47 - i] << i * 8);
    }

    start = le64toh(start);
    end = le64toh(end);
    req.start = start;
    req.end = end;
    req.priority = prio;
    req.con = connfd;

    return req;
}

void *hashThread(void *input)
{   
    Request req = *(Request *)input;
    uint64_t ret = reversehash(req.start, req.end, req.hash);

    if(ret == 0) {
        pthread_exit((void *)ret);
    }

    ret = htobe64(ret);
    pthread_exit((void *)ret);
    return NULL;
}

void *masterThread(void *input) {
    Lort lort = *(Lort *)input;
    Request_node *anchor_node = lort.arg1;
    hashArrayElem *oldHashResults = lort.oldHashResults;
    int id = lort.arg2;
    pthread_t thread_list[NUM_SLAVE_THREAD];

    while(1) {
        Request req = get_resuest(anchor_node);
        //printf("after rq \n");
        if (req.start == req.end) {
            sleep(1);
        } else {
            char out_buffer[PACKET_RESPONSE_SIZE];
            uint64_t value = oldHashCheck(req.hash, oldHashResults);
            //int value = -1;
            if (value != -1) {
                memcpy(out_buffer, &value, sizeof(out_buffer));
                write(req.con, out_buffer, sizeof(out_buffer));
            } else {
                Request* req_list[NUM_SLAVE_THREAD];
                int delta = (req.end - req.start) / NUM_SLAVE_THREAD;
                printf("DELTA IS %ld\n", delta);
                for(int i = 0; i < NUM_SLAVE_THREAD; i++) {
                    Request* request = malloc(sizeof(Request));
                    memcpy(request,&req,sizeof(Request));
                    req_list[i] = request;
                    (*request).start = req.start + delta*i;
                    printf("START IS %ld\n",(*request).start);
                    
                    if(i != NUM_SLAVE_THREAD-1) {
                        (*request).end = (*request).start + delta;
                    }
                    printf("END IS ",(*request).end);
                    pthread_create(&thread_list[i], NULL, hashThread, request);
                }
                uint64_t ret;
                for(int i = 0; i < NUM_SLAVE_THREAD; i++) {
                    uint64_t temp;
                    pthread_t thread_id = i;
                    pthread_join(thread_list[i],(void**)&temp);
                    if (temp > 0) {
                        ret = temp;
                    }
                    free(req_list[i]);
                }

                oldHashAdd(req.hash, ret, oldHashResults);

                memcpy(out_buffer, &ret, sizeof(out_buffer));
                write(req.con, out_buffer, sizeof(out_buffer));
            }
        }
    }
}

// Driver function
int main(int argc, char **argv)
{
    int sockfd, connfd, len, portno, threads;
    struct sockaddr_in servaddr, cli;
    char in_buffer[PACKET_REQUEST_SIZE];
    char out_buffer[PACKET_RESPONSE_SIZE];

    if (argc <= 1)
    {
        perror("ERROR give port");
        exit(1);
    }

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    
    bzero(&servaddr, sizeof(servaddr));

    portno = atoi(argv[1]);
    if (argc <= 2)
    {
        threads = 8;
    }
    else
    {
        threads = atoi(argv[2]);
    }

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portno);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("server accept failed...\n");
        exit(0);
    }
    else
    {
        printf("server accept the client...\n");
    }

    Request_node *anchor_node = create_anchor_node();

    // Create cache
    hashArrayElem *oldHashResults = initHashCache();

    printf("Created cache succeed!\n");

    // for (int i = 0; i < threads; ++i)
    // {
    //     Lort *lort = (Lort *)malloc(sizeof(Lort));
    //     //(*lort).arg1=anchor_node;
    //     //Lort templort = *lort;
    //     (*lort).arg1 = anchor_node;
    //     (*lort).arg2 = i;
    //     (*lort).oldHashResults = oldHashResults;
    //     //Lort *lort_pointer = lort;
    //     pthread_t thread_id = i;
    //     pthread_create(&thread_id, NULL, hashThread, lort);
    // }
    Lort *lort = (Lort *)malloc(sizeof(Lort));
    (*lort).arg1 = anchor_node;
    (*lort).arg2 = 0;
    (*lort).oldHashResults = oldHashResults;
    pthread_t thread_id = 4;
    pthread_create(&thread_id,NULL, masterThread, lort);

    while (connfd)
    {
        if (connfd < 0)
        {
            printf("accept failed");
            break;
        }
        else
        {
            Request req = new_request(connfd);
            printf("New request! \n");
            bzero(out_buffer, PACKET_RESPONSE_SIZE);
            uint64_t value = oldHashCheck(req.hash, oldHashResults);
            if (value != -1) {
                printf("SAME!");
                memcpy(out_buffer, &value, sizeof(out_buffer));
                write(req.con, out_buffer, sizeof(out_buffer));
            } else {
                Request_node *new_node = create_node(req);
                insert_node(anchor_node, new_node);
            }

            // uint64_t ret = reversehash(req.start, req.end, req.hash);
            // if(ret < (uint64_t)0) {
            //     printf("idk man\n");
            // }
            // ret = htobe64(ret);
            // memcpy(out_buffer, &ret,sizeof(out_buffer));
            // write(connfd,out_buffer,sizeof(out_buffer));

            // pthread_t thread_id;
            // pthread_create(&thread_id,NULL,hashThread,&req);
            // pthread_join(thread_id,NULL);
        }
        connfd = accept(sockfd, (SA *)&cli, &len);
    }
    delete_node(anchor_node);

    // After chatting close the socket
    close(sockfd);
}
