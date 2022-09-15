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
#define SA struct sockaddr
   

   
// Driver function
int main(int argc, char **argv)
{
    int sockfd, connfd, len, portno;
    struct sockaddr_in servaddr, cli;
    char buffer[PACKET_REQUEST_SIZE];

    if (argc <= 1) {
        perror("ERROR give port");
        exit(1);  
    }
   
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    portno = atoi(argv[1]);
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portno);
   
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
        printf("%d\n", servaddr.sin_addr.s_addr);
    len = sizeof(cli);
   
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else printf("server accept the client...\n");

    Request req;


      uint64_t start, end = 0;
      uint8_t prio = 0;
      bzero(buffer, PACKET_REQUEST_SIZE);
      read(connfd, buffer, sizeof(buffer));
      memcpy(req.hash, &buffer[PACKET_REQUEST_HASH_OFFSET], 32);
      
      for(int i = 0; i < 4; i++) {
        start |= (uint64_t)buffer[39-i] << i*8;
        end |= (uint64_t)buffer[47-i] << i*8;
      }

      start = le64toh(start);
      end = le64toh(end);

    printf("START IS %d END IS %d\n", start, end);
   
    // After chatting close the socket
    close(sockfd);
}