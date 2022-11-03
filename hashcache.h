#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define HASHTABELLENGTH 100

typedef struct hashArrayElem {
    uint64_t value;
    uint8_t hash[32];
} hashArrayElem;


hashArrayElem* initHashCache();
void resizeCache(hashArrayElem* oldHashResults);
void oldHashAdd(uint8_t hash[32], uint64_t key, hashArrayElem* oldHashResults);
uint64_t oldHashCheck(uint8_t hash[32], hashArrayElem* oldHashResults);