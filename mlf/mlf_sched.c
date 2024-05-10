#include "./mlf_queue.h"
#include "../queue.h"
#include "stddef.h"

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

void schedEnqueue(PCB *pcb)
{
    switch (pcb->priority)
    {
    case 1:
        mlfEnQueue(&queue1, pcb);
        break;
    case 2:
        mlfEnQueue(&queue2, pcb);
        break;
    case 3:
        mlfEnQueue(&queue3, pcb);
        break;
    case 4:
        mlfEnQueue(&queue4, pcb);
        break;
    default:
        break;
    }
    pcb->state = READY;
}



PCB *schedSelect()
{   
    PCB *pcb = NULL;

    if (!isEmpty(&queue1))
        pcb = mlfDeQueue(&queue1);
    else if (!isEmpty(&queue2))
        pcb = mlfDeQueue(&queue2);
    else if (!isEmpty(&queue3))
        pcb = mlfDeQueue(&queue3);
    else if (!isEmpty(&queue4))
        pcb = mlfDeQueue(&queue4);
    else return NULL;

    pcb->priority++;
    pcb->state = RUNNING;

    return pcb;
    

    
}

void schedBlock(PCB *pcb)
{
    pcb->state = BLOCKED;
    pcb->priority++;
    enQueue(&blockedQueue, pcb);
}

void unblock()
{
    PCB *pcb = deQueue(&blockedQueue);

    schedEnqueue(pcb);
}
