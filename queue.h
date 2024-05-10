#ifndef QUEUE_H
#define QUEUE_H

#include "pcb.h"

struct Queue {
	struct QNode *front, *rear;
	
};

struct Queue* createQueue();
void enQueue(struct Queue* q, PCB *k);
PCB *deQueue(struct Queue* q);

#endif