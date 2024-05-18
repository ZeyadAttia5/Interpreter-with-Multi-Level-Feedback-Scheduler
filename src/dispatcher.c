#include "dispatcher.h"
#include "mlf_sched.h"
#include "stddef.h"
#include "stdio.h"


int runningPid;
int runningQuantum;
int currentQueueQuantum;

void initDispatcher()
{
    runningPid = -1;
    runningQuantum = 0;
    currentQueueQuantum = 0;
}

void dispatch()
{

    int *dispatchedResult = schedSelect();

    if(dispatchedResult == NULL)
    {
        printf("No process to dispatch\n");
        return;
    }

    int selectedPid = dispatchedResult[0];

    currentQueueQuantum = dispatchedResult[1];

    printf("Selected PID: %d\n", selectedPid);

//    if(runningPid != -1) schedEnqueue(selectedPid);

    runningPid = selectedPid;

    printf("Running PID: %d\n", runningPid);

    printf("Running Quantum: %d\n", runningQuantum);

    printf("Current Queue Quantum: %d\n", currentQueueQuantum);



}

void setRunningQuantum(int quantum)
{
    runningQuantum = quantum;
}

void setRunningPid(int pid)
{
    runningPid = pid;
}

void setCurrentQueueQuantum(int quantum)
{
    currentQueueQuantum = quantum;
}

int getRunningPid()
{
    return runningPid;
}

int getRunningQuantum()
{
    return runningQuantum;
}

int getCurrentQueueQuantum()
{
    return currentQueueQuantum;
}
