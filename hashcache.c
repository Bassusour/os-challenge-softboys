#include "hashcache.h"

int sizeOfCache = 500;
int numOfElemAdded = 0;

pthread_mutex_t lockCache;


hashArrayElem* initHashCache() {
    hashArrayElem* oldHashResults = (hashArrayElem*) malloc(sizeOfCache, sizeof(hashArrayElem));
    
    if (pthread_mutex_init(&lockCache, NULL) != 0) {
        return NULL;
    }
    
    return oldHashResults;
}

void resizeCache(hashArrayElem* oldHashResults) {
    int oldSizeOfCache = sizeOfCache;
    sizeOfCache *= 2;
    hashArrayElem* newHashResultArray = (hashArrayElem*) malloc(sizeOfCache, sizeof(hashArrayElem));
    
    for (int i = 0; i < oldSizeOfCache; i++) {
        //uint8_t hash[32] = oldHashResults[i].hash;
        uint64_t value = oldHashResults[i].value;
        int num = 0;
        for (int j = 0; j < 32; j++) {
            num = (num +  oldHashResults[i].hash[j]) % sizeOfCache;
        }
        if (newHashResultArray[num].value != 0) {
        while (1) {
            // probing by 1
            num = (num + 1) % sizeOfCache;
            if (newHashResultArray[num].value == 0) {
                memcpy(newHashResultArray[num].hash, oldHashResults[i].hash, sizeof(oldHashResults[i].hash));
                newHashResultArray[num].value = value;
                return;
            }
        }
        } else {
            memcpy(newHashResultArray[num].hash, oldHashResults[i].hash, sizeof(oldHashResults[i].hash));
            newHashResultArray[num].value = value;
        }
    }
    free(oldHashResults);
    oldHashResults = newHashResultArray;
}

void oldHashAdd(uint8_t hash[32], uint64_t value, hashArrayElem* oldHashResults) {
    pthread_mutex_lock(&lockCache);

    // Generate hash for cache
    int num = 0;
    for (int i = 0; i < 32; i++) {
        num = (num + hash[i]) % sizeOfCache;
    }

    if (oldHashResults[num].value != 0) {
        while (1) {
            // probing by 1
            num = (num + 1) % sizeOfCache;
            if (oldHashResults[num].value == 0) {
                memcpy(oldHashResults[num].hash, hash, sizeof(hash));
                oldHashResults[num].value = value;
                break;
            }
        }
    } else {
        memcpy(oldHashResults[num].hash, hash, sizeof(hash));
        oldHashResults[num].value = value;
    }
    numOfElemAdded++;
    if (numOfElemAdded >= sizeOfCache/2) {
        resizeCache(oldHashResults);
    }
    pthread_mutex_unlock(&lockCache);
}

uint64_t oldHashCheck(uint8_t hash[32], hashArrayElem* oldHashResults) {
    pthread_mutex_lock(&lockCache);
    int num = 0;
    for (int i = 0; i < 32; i++) {
        num = (num + hash[i]) % sizeOfCache;
    }
    printf("%d cache hash num \n", num);
    while (1) {
        int64_t value = oldHashResults[num].value;
        printf("%d the check value \n", value);
        if (value != 0) {
            if (!memcmp(hash,oldHashResults[num].hash, sizeof(hash))) {
                pthread_mutex_unlock(&lockCache);
                return value;
            } else {
                num = num + 1;
            }
        } else {
            pthread_mutex_unlock(&lockCache);
            return -1;
        }
      
    }
    
}
