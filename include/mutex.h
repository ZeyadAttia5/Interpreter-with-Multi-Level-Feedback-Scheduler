#ifndef OS_PROJECT_MUTEX_H
#define OS_PROJECT_MUTEX_H
#define LOCKED 0
#define UNLOCKED 1
#include "queue.h"

typedef struct mutex {
    int value;
    struct Queue *queue;
    char *resourceName;
    int pid;
} mutex;

void create_mutex(int initialValue, char* resourceName, mutex **s);

void wait_mutex(mutex *s, int pid);

int signal_mutex(mutex *s);

void print_mutex_queue(mutex *s);

int try_wait_mutex(mutex *s, int pid);

#endif //OS_PROJECT_MUTEX_H
