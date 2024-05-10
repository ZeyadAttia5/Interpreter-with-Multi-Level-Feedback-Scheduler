#include "./mlf_queue.h"
#include "../queue.h"

//create the mlf sched
MlfQueue queue1, queue2, queue3, queue4;
struct Queue *blockedQueue;


int main()
{
    createMlfQueue(&queue1, 1);
    createMlfQueue(&queue2, 2);
    createMlfQueue(&queue3, 4);
    createMlfQueue(&queue4, 8);

    blockedQueue = createQueue();
}




