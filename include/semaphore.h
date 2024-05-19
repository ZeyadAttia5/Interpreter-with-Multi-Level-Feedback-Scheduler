#ifndef OS_PROJECT_SEMAPHORE_H
#define OS_PROJECT_SEMAPHORE_H
#define LOCKED 0
#define UNLOCKED 1
#include "queue.h"

typedef struct semaphore {
    int value;
    struct Queue *queue;
    char *resourceName;
} semaphore;

void create_semaphore(int initialValue, char* resourceName, semaphore **s);

void wait_semaphore(semaphore *s, int pid);

int signal_semaphore(semaphore *s);

void print_semaphore_queue(semaphore *s);

int try_wait_semaphore(semaphore *s, int pid);

#endif //OS_PROJECT_SEMAPHORE_H
