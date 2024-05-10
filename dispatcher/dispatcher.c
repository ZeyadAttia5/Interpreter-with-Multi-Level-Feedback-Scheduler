#include "./dispatcher.h"
#include "../pcb.h"
#include "../mlf/mlf_sched.h"
#include "stddef.h"

PCB *runningPCB = NULL;


void dispatch()
{

    PCB *selectedPcb = schedSelect();

    if(runningPCB != NULL) schedEnqueue(runningPCB);

    runningPCB = selectedPcb;



}
