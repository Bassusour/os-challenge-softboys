#include <stdint.h>

#ifndef SERVER_H
#define SERVER_H

typedef struct {
    uint8_t hash[32];
    uint64_t start;
    uint64_t end;
    uint8_t priority;
} Request;

Request new_request(int connfd);

#endif