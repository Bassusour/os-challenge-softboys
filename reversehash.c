#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <openssl/ssl.h>
#include <stdbool.h>

uint64_t reversehash(uint64_t start, uint64_t end, uint8_t hash[32]){

    // In and out buffers 
    unsigned char obuf[32];
    unsigned char ibuf[sizeof(start)];
    for(uint64_t num = start; num <= end; num++){

        // bzero(ibuf,sizeof(ibuf));
        memcpy(ibuf, &num, sizeof(num));

        SHA256(ibuf, sizeof(ibuf), obuf);

        // Return when result is found
        if(!memcmp(obuf,hash,sizeof(obuf))){
            return num;
        }

        // Reset buffers
        bzero(ibuf,sizeof(ibuf));
        bzero(obuf,sizeof(obuf));
    }
    return -1;
}