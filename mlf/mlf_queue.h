#ifndef MLF_QUEUE_H
#define MLF_QUEUE_H

#include "../queue.h"

typedef struct MlfQueue
{
    struct Queue *queue;
    int quantum;

} MlfQueue;

void createMlfQueue(MlfQueue *queue, int quantum);
void mlfEnQueue(MlfQueue *queue, PCB *key);
PCB *mlfDeQueue(MlfQueue *queue);
int isEmpty(MlfQueue *queue);

#endif