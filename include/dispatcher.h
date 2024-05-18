#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "stddef.h"




void initDispatcher();
void dispatch();
void setRunningPid(int pid);
void setRunningQuantum(int quantum);
void setCurrentQueueQuantum(int quantum);
int getRunningPid();
int getRunningQuantum();
int getCurrentQueueQuantum();

#endif
