typedef int spinlock;

#define UNLOCKED 0
#define LOCKED 1

void spinlock_init(spinlock* lock); 

void spinlock_lock(spinlock* lock);

void spinlock_unlock(spinlock* lock);
