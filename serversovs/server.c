#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>


int main(int argc, char **argv){
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;

    if (argc <= 1) {
        perror("ERROR give port");
        exit(1);  
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
    }

    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
    }

    bzero(buffer,256);
    n = read( newsockfd,buffer,255 );
   
    if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
    }
   
    printf("Here is the message: %s\n",buffer);

   /* Write a response to the client */
    n = write(newsockfd,"I got your message",18);
   
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }


    
    printf("The reverse hash is 367866678");
    return 0;
}