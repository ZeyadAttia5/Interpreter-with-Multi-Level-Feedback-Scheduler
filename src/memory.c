#include "memory.h"
#include "strings.h"
#include "string.h"
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

    setMemoryWordValue(pcbBaseAddress + offset, value);

}

void addNewPCB(char *upperBound, char *lowerBound)
{
    PCB_COUNT++;

    int pcbBaseAddress = MEMORY_SIZE - PCB_COUNT * PCB_SIZE;

    char pcbCountString[12];
    sprintf(pcbCountString, "%d", PCB_COUNT);

    setPCBField("PID", PCB_COUNT, pcbCountString);

    setPCBField("STATE", PCB_COUNT, "READY");

    setPCBField("PRIORITY", PCB_COUNT, "1");

    setPCBField("PC", PCB_COUNT, "0");

    setPCBField("UPPER_BOUND", PCB_COUNT, upperBound);

    setPCBField("LOWER_BOUND", PCB_COUNT, lowerBound);

}

void printMemory()
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        printf("Memory[%d]: %s %s\n", i, memory[i].name, memory[i].value);
    }
}