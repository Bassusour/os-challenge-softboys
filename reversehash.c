#include <stdio.h>
#include <stdint.h>

uint64_t reversehash(uint64_t start, uint64_t end, uint8_t hash[32]){
    for(uint64_t i = start; i <= end; i++){
        uint8_t buffer[32];




        if(!strcmp(buffer,hash)){
            return i;
        }
    }



    return -1;
}