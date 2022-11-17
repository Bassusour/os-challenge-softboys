#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <openssl/ssl.h>
#include <stdbool.h>

uint64_t reversehash(uint64_t start, uint64_t end, uint8_t hash[32]){

    unsigned char obuf[32];
    unsigned char ibuf[sizeof(start)];
    for(uint64_t num = start; num <= end; num++){

        // bzero(ibuf,sizeof(ibuf));
        memcpy(ibuf, &num, sizeof(num));
        

        //printf("%d \n",strlen(ibuf) );
        SHA256(ibuf, sizeof(ibuf), obuf);

        if(!memcmp(obuf,hash,sizeof(obuf))){
            return num;
        }
        bzero(ibuf,sizeof(ibuf));
        bzero(obuf,sizeof(obuf));
    }
    return 0;
}