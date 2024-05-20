#include <stdlib.h>
#include "dispatcher.h"
#include "mlf_sched.h"
#include "stddef.h"
#include "stdio.h"
#include "mutex.h"
#include "insrtuction_set.h"

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

    runningPid = -1;

    int *dispatchedResult = schedSelect();

    if(dispatchedResult == NULL)
    {
//        printf("No process to dispatch\n");
        return;
    }

    int selectedPid = dispatchedResult[0];

    currentQueueQuantum = dispatchedResult[1];

    runningQuantum = 0;



//    if(runningPid != -1) schedEnqueue(selectedPid);

    runningPid = selectedPid;

    // get priority of the selected process
    int priority = atoi(getPCBField("PRIORITY", selectedPid).value) - 1;

    printf("---------------------- Running PID: %d, WITH PRIORITY %d ----------------------\n", getRunningPid(), priority);
    printQueues();





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
