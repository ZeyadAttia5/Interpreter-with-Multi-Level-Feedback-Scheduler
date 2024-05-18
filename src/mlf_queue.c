#include "mlf_queue.h"
#include <stddef.h>




void createMlfQueue(MlfQueue *queue, int quantum)
{
    queue->queue = createQueue();
    queue->quantum = quantum;
}

void mlfEnQueue(MlfQueue *queue, int key)
{
    enQueue(queue->queue, key);


}

int mlfDeQueue(MlfQueue *queue)
{
    return deQueue(queue->queue);
}

int isMlfEmpty(MlfQueue *queue)
{
    return queue->queue->front == NULL;
}

void printMlfQueue(MlfQueue *queue)
{
    printQueue(queue->queue);
}
