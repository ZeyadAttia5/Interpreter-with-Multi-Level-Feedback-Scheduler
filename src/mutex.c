#include "../include/mutex.h"
#include "stdlib.h"
#include "stdio.h"
#include "mlf_sched.h"
#include "dispatcher.h"


void create_mutex(int initialValue, char* resourceName, mutex **s)
{
    if(initialValue != LOCKED && initialValue != UNLOCKED)
    {
        printf("Invalid initial value for semaphore\n");
        return;
    }

    *s = (mutex *)malloc(sizeof(mutex));
    (*s)->value = initialValue;
    (*s)->queue = createQueue();
    (*s)->resourceName = resourceName;

}

void wait_mutex(mutex *s, int pid)
{
    if(s->value == LOCKED)
    {
        enQueue(s->queue, pid);

    }else
    {
        printf("Process %d acquired resource %s\n", pid, s->resourceName);
        s->value = LOCKED;
        s->pid = pid;
    }
}

int signal_mutex(mutex *s)
{
    if (s->pid != getRunningPid() && s->pid != -1)
    {
        printf("Process %d is not the owner of the resource %s\n", getRunningPid(), s->resourceName);
        return -1;
    }
    s->value = UNLOCKED;
    printf("Resource %s released\n", s->resourceName);

    int unblockedPid = dequeueHighestPriority(s->queue);

    schedRemoveBlockedKey(unblockedPid);

    s->pid = -1;



    return unblockedPid;

}

void print_mutex_queue(mutex *s)
{
    printQueue(s->queue);
}


int try_wait_mutex(mutex *s, int pid)
{
    if(s->value == LOCKED)
    {
        enQueue(s->queue, pid);
        schedBlock(pid);
        return 0;
    }else
    {
        printf("Process %d acquired resource %s\n", pid, s->resourceName);
        s->value = LOCKED;
        s->pid = pid;

        return 1;
    }
}