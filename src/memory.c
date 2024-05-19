#include "memory.h"
#include "strings.h"
#include "string.h"
#include "STRING_OPERATIONS.h"

#define _OPEN_SYS_ITOA_EXT
#include <stdlib.h>
#include <stdio.h>



static MemoryWord memory[MEMORY_SIZE];

static int PCB_COUNT = 0;

void initMemory()
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        memory[i].name = "";
        memory[i].value = "";
    }
}

void setMemoryWholeWord(int address, char *name, char *value)
{
    if (address < 0 || address >= MEMORY_SIZE)
    {
        printf("Memory address out of bounds\n");
        return;
    }
    memory[address].name = name;
    memory[address].value = value;
}

void setMemoryWordValue(int address, char *value)
{
    if (address < 0 || address >= MEMORY_SIZE)
    {
        printf("Memory address out of bounds\n");
        return;
    }
    memory[address].value = value;
}

MemoryWord getMemoryWord(int address)
{
    if (address < 0 || address >= MEMORY_SIZE)
    {
        printf("Memory address out of bounds\n");
        MemoryWord empty;
        empty.name = "";
        empty.value = "";
        return empty;
    }
    return memory[address];
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

void removeProcess(int pcbIndex)
{

    // Clear memory from lower bound to upper bound
    int upperBound = atoi(getPCBField("UPPER_BOUND", pcbIndex).value);
    int lowerBound = atoi(getPCBField("LOWER_BOUND", pcbIndex).value);
    int pcbBaseAddress = MEMORY_SIZE - pcbIndex * PCB_SIZE;

    for (int i = 0; i < PCB_SIZE; i++)
    {
        memory[pcbBaseAddress + i].name = "";
        memory[pcbBaseAddress + i].value = "";
    }



    for (int i = lowerBound; i <= upperBound; i++)
    {
        memory[i].name = "";
        memory[i].value = "";
    }

//    PCB_COUNT--;
}

void printMemory()
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        printf("Memory[%d]: %s %s\n", i, memory[i].name, memory[i].value);
    }
}

char *getVariableValue(char *variableName, int runningPID)
{

    int upperBound = atoi(getPCBField("UPPER_BOUND", runningPID).value);
    int lowerBound = atoi(getPCBField("LOWER_BOUND", runningPID).value);


    for (int i = lowerBound; i <= upperBound; i++)
    {
        if (memory[i].name != NULL && !strcasecmp(memory[i].name, variableName))
        {
            return memory[i].value;
        }
    }
    return NULL;
}