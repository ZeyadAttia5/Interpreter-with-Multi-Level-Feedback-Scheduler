#ifndef MLF_QUEUE_H
#define MLF_QUEUE_H

#include "queue.h"

typedef struct MlfQueue
{
    struct Queue *queue;
    int quantum;

} MlfQueue;

void createMlfQueue(MlfQueue *queue, int quantum);
void mlfEnQueue(MlfQueue *queue, int key);
int mlfDeQueue(MlfQueue *queue);
int isMlfEmpty(MlfQueue *queue);
void printMlfQueue(MlfQueue *queue);

#endif