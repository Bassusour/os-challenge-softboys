
#include <assert.h>
#include <stdlib.h>
#include "spinlock.h"

void spinlock_init(spinlock* lock) {
    /* TODO: Implement */
    *lock = UNLOCKED;  

}

void spinlock_lock(spinlock* lock) {
    /* TODO: Implement */
    while(!__sync_val_compare_and_swap(lock,UNLOCKED,LOCKED))
    {}
}

void spinlock_unlock(spinlock* lock) {
    // assert(*lock == LOCKED);
    __sync_val_compare_and_swap(lock,LOCKED,UNLOCKED);

    //while(__sync_val_compare_and_swap(lock,LOCKED,UNLOCKED) == LOCKED){
    //  *lock = 0;  
    //}
}
