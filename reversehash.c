#include <stdio.h>
#include <stdint.h>

uint64_t reversehash(uint64_t start, uint64_t end, uint8_t hash[32]){
    for(uint64_t num = start; num <= end; num++){

        unsigned char ibuf[sizeof(num)];
        memcpy(ibuf, &num, sizeof(num));
        
        unsigned char obuf[32];

        //printf("%d \n",strlen(ibuf) );
        SHA256(ibuf, sizeof(ibuf), obuf);

        // int i;
        // for (i = 0; i < 32; i++) {
        //     printf("%02x ", obuf[i]);
        // }
        // printf("\n");

        if(!strcmp(obuf,hash)){
            return num;
        }
    }

    return -1;
}