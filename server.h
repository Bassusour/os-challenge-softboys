#include <stdint.h>

#ifndef SERVER_H
#define SERVER_H

#define NUM_SLAVE_THREAD 4

typedef struct {
    uint8_t hash[32];
    uint64_t start;
    uint64_t end;
    uint8_t priority;
    uint8_t result;
    int con;
} Request;

Request new_request(int connfd);

#endif