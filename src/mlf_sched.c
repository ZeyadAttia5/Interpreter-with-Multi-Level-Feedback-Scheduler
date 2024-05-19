#include "mlf_queue.h"
#include "queue.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "dispatcher.h"
#include "memory.h"
#include "string.h"
#include "STRING_OPERATIONS.h"

// create the mlf sched
MlfQueue queue1, queue2, queue3, queue4;
struct Queue *blockedQueue;

int initializeScheduler()
{
    createMlfQueue(&queue1, 1);
    createMlfQueue(&queue2, 2);
    createMlfQueue(&queue3, 4);
    createMlfQueue(&queue4, 8);

    blockedQueue = createQueue();
}

void schedEnqueue(int pid)
{
    int priority = atoi(getPCBField("PRIORITY", pid).value);
    switch (priority)
    {
    case 1:
        mlfEnQueue(&queue1, pid);
        break;
    case 2:
        mlfEnQueue(&queue2, pid);
        break;
    case 3:
        mlfEnQueue(&queue3, pid);
        break;
    case 4:
        mlfEnQueue(&queue4, pid);
        break;
    default:
        break;
    }

    setPCBField("STATE", pid, "READY");
}



int *schedSelect()
{   
    int pid = 0;
    int queueQuantum = 0;

    if (!isMlfEmpty(&queue1)) {
        pid = mlfDeQueue(&queue1);
        queueQuantum = queue1.quantum;
    }else if (!isMlfEmpty(&queue2)) {
        pid = mlfDeQueue(&queue2);
        queueQuantum = queue2.quantum;
    }else if (!isMlfEmpty(&queue3)) {
        pid = mlfDeQueue(&queue3);
        queueQuantum = queue3.quantum;
    }else if (!isMlfEmpty(&queue4)) {
        pid = mlfDeQueue(&queue4);
        queueQuantum = queue4.quantum;
    }else return NULL;



    int priority = atoi(getPCBField("PRIORITY", pid).value);
    priority++;

    char *priorityStr = malloc(sizeof(char) * 12);

    itoa(priority, priorityStr, 10);



    setPCBField("PRIORITY", pid, priorityStr);
    setPCBField("STATE", pid, "RUNNING");


    int *result = malloc(sizeof(int) * 2);

    result[0] = pid;
    result[1] = queueQuantum;

    return result;
    

    
}

void schedBlock(int pid)
{

    int priority = atoi(getPCBField("PRIORITY", pid).value);
    if(getRunningQuantum() == pow(2, priority))
    {
        priority++;
    }

    char *priorityStr = malloc(sizeof(char) * 10);

    itoa(priority, priorityStr, 10);

    setPCBField("STATE", pid,  "BLOCKED");
    setPCBField("PRIORITY", pid, priorityStr);

    enQueue(blockedQueue, pid);
}

void unblock()
{
    int pid = deQueue(blockedQueue);

    schedEnqueue(pid);
}


void printQueues()
{
    printf("Queue 1: ");
    printMlfQueue(&queue1);
    printf("\n");

    printf("Queue 2: ");
    printMlfQueue(&queue2);
    printf("\n");

    printf("Queue 3: ");
    printMlfQueue(&queue3);
    printf("\n");

    printf("Queue 4: ");
    printMlfQueue(&queue4);
    printf("\n");

//    printf("Blocked Queue: ");
//    printQueue(blockedQueue);
//    printf("\n");
}

void try_unblock(char *resource)
{
    int size = blockedQueue->size;

    for(int i = 0; i < size; i++)
    {
        int dequeuedPid = deQueue(blockedQueue);

        int PC = atoi(getPCBField("PC", dequeuedPid).value);

        PC--;

        char *processResource = getMemoryWord(PC).value;

        if(!strcmp(processResource, resource))
        {
            schedEnqueue(dequeuedPid);
        }
        else
        {
            enQueue(blockedQueue, dequeuedPid);
        }

    }
}