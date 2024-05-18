#ifndef MLF_SCHED_H
#define MLF_SCHED_H

int initializeScheduler();


void schedEnqueue(int pid);

void schedDequeue(int pid);

int *schedSelect();

void schedBlock(int pid);

void unblock();

void printQueues();

void try_unblock(char *resource);

#endif