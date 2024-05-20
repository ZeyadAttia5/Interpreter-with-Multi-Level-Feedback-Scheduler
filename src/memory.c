#include "memory.h"
#include "strings.h"
#include "string.h"
#include "STRING_OPERATIONS.h"

#define _OPEN_SYS_ITOA_EXT
#include <stdlib.h>
#include <stdio.h>



static MemoryWord memory[MEMORY_SIZE];



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
        if (memory[i].name != NULL && strlen(memory[i].name) > 0)
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

