#include "stdio.h"
#include "queue.h"





typedef struct Instruction
{
    char *name;
    char *value;
} Instruction;

typedef struct Memory
{
   PCB pcbs[3];
   Instruction instructions[30];
   char *linesOfCode[30];
} Memory;








