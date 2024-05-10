#include "./mlf_queue.h"
#include <stddef.h>




void createMlfQueue(MlfQueue *queue, int quantum)
{
    queue->queue = createQueue();
    queue->quantum = quantum;
}

void mlfEnQueue(MlfQueue *queue, PCB *key)
{
    enQueue(queue->queue, key);


}

PCB *mlfDeQueue(MlfQueue *queue)
{
    return deQueue(queue->queue);
}

int isEmpty(MlfQueue *queue)
{
    return queue->queue->front == NULL;
}


