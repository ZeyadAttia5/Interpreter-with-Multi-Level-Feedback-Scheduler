#ifndef QUEUE_H
#define QUEUE_H


struct Queue {
	struct QNode *front, *rear;
    int size;
	
};

struct Queue* createQueue();
void enQueue(struct Queue* q, int k);
int deQueue(struct Queue* q);
void printQueue(struct Queue* q);

#endif