#include "pcb.h"
#include "string.h"
#include "stdlib.h"
#include "memory.h"
#include "STRING_OPERATIONS.h"
#include "stdio.h"

static int PCB_COUNT = 0;
void incrementPC(int runningPID)
{
    int pc = atoi(getPCBField("PC", runningPID).value);
    pc++;
    char *pcString = (char *)malloc(10 * sizeof(char));
    itoa(pc, pcString, 10);
    setPCBField("PC", runningPID, pcString);
}

void decrementPC(int runningPID)
{
    int pc = atoi(getPCBField("PC", runningPID).value);
    pc--;
    char *pcString = (char *)malloc(10 * sizeof(char));
    itoa(pc, pcString, 10);
    setPCBField("PC", runningPID, pcString);
}

int getPCBOffset(char *fieldName)
{

    int offset = 0;

    if (!strcasecmp(fieldName, "STATE"))
        offset = 1;
    else if (!strcasecmp(fieldName, "PRIORITY"))
        offset = 2;
    else if (!strcasecmp(fieldName, "PC"))
        offset = 3;
    else if (!strcasecmp(fieldName, "UPPER_BOUND"))
        offset = 4;
    else if (!strcasecmp(fieldName, "LOWER_BOUND"))
        offset = 5;

    return offset;
}



MemoryWord getPCBField(char *fieldName, int pcbIndex)
{
    if (pcbIndex < 0 || pcbIndex > PCB_COUNT)
    {
        printf("PCB index out of bounds\n");

        MemoryWord empty;
        empty.name = "";
        empty.value = "";
        return empty;
    }
    int pcbBaseAddress = MEMORY_SIZE - pcbIndex * PCB_SIZE;

    int offset = getPCBOffset(fieldName);


    return getMemoryWord(pcbBaseAddress + offset);

}

void setPCBField(char *fieldName, int pcbIndex, char *value)
{
    if (pcbIndex < 0 || pcbIndex > PCB_COUNT)
    {
        printf("PCB index out of bounds\n");
        return;
    }

    int pcbBaseAddress = MEMORY_SIZE - pcbIndex * PCB_SIZE;

    int offset = getPCBOffset(fieldName);

    setMemoryWholeWord(pcbBaseAddress + offset, fieldName, value);

}

void addNewPCB(char *upperBound, char *lowerBound)
{
    PCB_COUNT++;

    int pcbBaseAddress = MEMORY_SIZE - PCB_COUNT * PCB_SIZE;



    char *pcbCountString = (char *)malloc(10 * sizeof(char));
    itoa(PCB_COUNT, pcbCountString, 10);

    setPCBField("PID", PCB_COUNT, pcbCountString);

    setPCBField("STATE", PCB_COUNT, "READY");

    setPCBField("PRIORITY", PCB_COUNT, "1");

    setPCBField("PC", PCB_COUNT, lowerBound);

    setPCBField("UPPER_BOUND", PCB_COUNT, upperBound);

    setPCBField("LOWER_BOUND", PCB_COUNT, lowerBound);


}