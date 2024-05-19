#ifndef PCB_H
#define PCB_H
#include "memory.h"

void incrementPC(int runningPID);
void decrementPC(int runningPID);
int getPCBOffset(char *fieldName);
MemoryWord getPCBField(char *fieldName, int pcbIndex);
void setPCBField(char *fieldName, int pcbIndex, char *value);
void addNewPCB(char *upperBound, char *lowerBound);



#endif