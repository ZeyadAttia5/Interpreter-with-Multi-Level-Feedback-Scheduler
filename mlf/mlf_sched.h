#include "../pcb.h"

int initializeScheduler();


void schedEnqueue(PCB *pcb);

void schedDequeue(PCB *pcb);

PCB *schedSelect();

void schedBlock(PCB *pcb);

void unblock();