#include "insrtuction_set.h"
#include "memory.h"
#include <stdio.h>
#include "dispatcher.h"
#include <stdlib.h>
#include <string.h>
#include "pthread.h"
#include "mlf_sched.h"
#define LOCKED 0
#define UNLOCKED 1


//int runningPid;
//int runningQuantum;

int outputBufferMutex = UNLOCKED, fileBufferMutex = UNLOCKED, inputBufferMutex = UNLOCKED;

void print(char *x)
{
    printf("%s\n", x);
}
void assign(char *x, char *y)
{
    int lowerBound = atoi(getPCBField("LOWER_BOUND", getRunningPid()).value);

    int upperBound = atoi(getPCBField("UPPER_BOUND", getRunningPid()).value);

    for (int i = lowerBound; i < upperBound; i++)
    {
        if(!strcmp(getMemoryWord(i).name, x))
        {
            setMemoryWordValue(i, y);
            break;
        }
    }



}
void writeFile(char *filename, char *data)
{
    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "%s", data);
    fclose(f);
}
char *readFile(char *filename)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);

    return string;

}

void printFromTo(int x, int y)
{
    for(int i = x; i <= y; i++)
    {
        printf("%d\n", i);
    }
}
void semWait(char *x)
{
     if (!strcmp(x, "userInput")){
         if(inputBufferMutex == LOCKED)
         {
             schedBlock(getRunningPid());
             dispatch();
         }else
         {
             inputBufferMutex = LOCKED;
         }
     }else if (!strcmp(x, "userOutput")) {
         if (outputBufferMutex == LOCKED) {
             schedBlock(getRunningPid());
             dispatch();
         }
         else
         {
             outputBufferMutex = LOCKED;
         }
     } else if (!strcmp(x, "file")) {
         if (fileBufferMutex == LOCKED) {
             schedBlock(getRunningPid());
             dispatch();
         }
         else
         {
             fileBufferMutex = LOCKED;
         }
     }


}





void semSignal(char *x)
{

    if (!strcmp(x, "userInput")) {
        inputBufferMutex = UNLOCKED;
        try_unblock(x);
    } else if (!strcmp(x, "userOutput")) {
        outputBufferMutex = UNLOCKED;
        try_unblock(x);
    } else if (!strcmp(x, "file")) {
        fileBufferMutex = UNLOCKED;
        try_unblock(x);
    }


}

void executeInstruction(MemoryWord currentInstruction)
{
    char *instruction = currentInstruction.name;

    char *args = currentInstruction.value;

    if(!strcmp(instruction, "print")) {
        print(args);
    }else if(!strcmp(instruction, "readFile")) {
        readFile(args);
    }else if(!strcmp(instruction, "writeFile")) {

        char *filename = strtok(args, " ");
        char *data = strtok(NULL, " ");
        writeFile(filename, data);

    } else if (!strcmp(instruction, "printFromTo")) {
        char *x = strtok(args, " ");
        char *y = strtok(NULL, " ");
        printFromTo(atoi(x), atoi(y));
    } else if (!strcmp(instruction, "assign")) {
        char *x = strtok(args, " ");
        char *y = strtok(NULL, " ");
        assign(x, y);
    } else if (!strcmp(instruction, "semWait")) {
        semWait(args);
    } else if (!strcmp(instruction, "semSignal")) {
        semSignal(args);
    }

}