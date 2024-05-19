#include "../include/semaphore.h"
#include "stdlib.h"
#include "stdio.h"



void create_semaphore(int initialValue, char* resourceName, semaphore **s)
{
    if(initialValue != LOCKED && initialValue != UNLOCKED)
    {
        printf("Invalid initial value for semaphore\n");
        return;
    }

    *s = (semaphore *)malloc(sizeof(semaphore));
    (*s)->value = initialValue;
    (*s)->queue = createQueue();
    (*s)->resourceName = resourceName;

}

void wait_semaphore(semaphore *s, int pid)
{
    if(s->value == LOCKED)
    {
        enQueue(s->queue, pid);
    }else
    {
        printf("Process %d acquired resource %s\n", pid, s->resourceName);
        s->value = LOCKED;
    }
}

int signal_semaphore(semaphore *s)
{
    s->value = UNLOCKED;
    printf("Resource %s released\n", s->resourceName);

    int unblockedPid = deQueue(s->queue);

    return unblockedPid;

}

void print_semaphore_queue(semaphore *s)
{
    printQueue(s->queue);
}
