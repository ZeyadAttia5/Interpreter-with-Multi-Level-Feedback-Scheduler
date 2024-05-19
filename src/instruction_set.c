#include "insrtuction_set.h"
#include "memory.h"
#include <stdio.h>
#include "dispatcher.h"
#include <stdlib.h>
#include <string.h>
#include "strings.h"
#include "pthread.h"
#include "mlf_sched.h"
#include "STRING_OPERATIONS.h"
#include "../include/semaphore.h"
#include "pcb.h"

semaphore *inputBufferSemaphore, *outputBufferSemaphore, *fileBufferSemaphore;

void instruction_set_init() {

    create_semaphore(1, "inputBuffer", &inputBufferSemaphore);
    create_semaphore(1, "outputBuffer", &outputBufferSemaphore);
    create_semaphore(1, "fileBuffer", &fileBufferSemaphore);

}

void print(char *x)
{
    printf("%s\n", x);
}
void assign(char *x, char *y)
{
    if (!strcmp("input", y))
    {
        char *input = malloc(100);
        // TODO: MUST ADD "Please enter a value for x" before scanf but doesn't work
        printf("Please enter a value for %s: ", x);
        scanf("%[^\n]%*c", input);
        y = input;
    }

    int lowerBound = atoi(getPCBField("LOWER_BOUND", getRunningPid()).value);

    int upperBound = atoi(getPCBField("UPPER_BOUND", getRunningPid()).value);

    for (int i = lowerBound; i <= upperBound; i++)
    {

        if(getMemoryWord(i).name != NULL && !strcmp(getMemoryWord(i).name, x))
        {

            setMemoryWholeWord(i, x, y);
            return;
        }
    }


    // check if word is not empty
    if (strlen(getMemoryWord(upperBound).name) != 0)
    {
        upperBound--;
    }
    // check if word is not empty
    if (strlen(getMemoryWord(upperBound).name) != 0)
    {
        upperBound--;
    }

    setMemoryWholeWord(upperBound, x, y);





}
void writeFile(char *filename, char *data)
{
    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
//        exit(1);
        return;
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
    int pid = getRunningPid();
     if (!strcmp(x, "userInput")){
         if(try_wait_semaphore(inputBufferSemaphore, pid) == 0){
             // decrement the pc of this prcoess to execute the instruction again when process wakes up
             decrementPC(pid);
             setRunningPid(-1); // to force the dispatcher to run the next process
             setRunningQuantum(1); // to force the dispatcher to run the next process

         }
     }else if (!strcmp(x, "userOutput")) {
         if (try_wait_semaphore(outputBufferSemaphore, pid) == 0) {
             // decrement the pc of this prcoess to execute the instruction again when process wakes up
             decrementPC(pid);
             setRunningPid(-1); // to force the dispatcher to run the next process
             setRunningQuantum(1); // to force the dispatcher to run the next process

         }
     } else if (!strcmp(x, "file")) {
            if (try_wait_semaphore(fileBufferSemaphore, pid) == 0) {
                // decrement the pc of this prcoess to execute the instruction again when process wakes up

                decrementPC(pid);
                setRunningPid(-1); // to force the dispatcher to run the next process
                setRunningQuantum(1); // to force the dispatcher to run the next process
            }
     }else{
         printf("Invalid semaphore name\n");
         return;
     }






}





void semSignal(char *x)
{
    int pid = -1;

    if (!strcmp(x, "userInput")) {
        pid = signal_semaphore(inputBufferSemaphore);
    } else if (!strcmp(x, "userOutput")) {
        pid = signal_semaphore(outputBufferSemaphore);
    } else if (!strcmp(x, "file")) {
        pid = signal_semaphore(fileBufferSemaphore);
    }else{
        printf("Invalid semaphore name\n");
        return;
    }

    if (pid != -1)
        schedEnqueue(pid);

}

void handleNestedInstruction(char *instruction, char *args, int *isNested, char **nestedOutput)
{




    // check if "readFile" is in args
    if (strstr(args, "readFile") != NULL)
    {

        char *argsCopy = strdup(args);
        char **tokens = str_split(argsCopy, ' ');
        *isNested = 1;
        MemoryWord nestedInstruction;



        char *file_data = readFile(getVariableValue(tokens[2], getRunningPid()));

        *nestedOutput = file_data;


        nestedInstruction.name = instruction;
        nestedInstruction.value = strcat(tokens[0], " ");
        nestedInstruction.value = strcat(nestedInstruction.value, file_data);

        executeInstruction(nestedInstruction);


    }



}

void executeInstruction(MemoryWord currentInstruction)
{

    int isNested = 0;
    char *nestedOutput;



    char *instruction = currentInstruction.name;

    char *args = currentInstruction.value;

    handleNestedInstruction(instruction, args, &isNested, &nestedOutput);

    if (isNested)
    {
        return;
    }

    if(!strcmp(instruction, "print")) {
        print(getVariableValue(args, getRunningPid()));
    }else if(!strcmp(instruction, "readFile")) {
        readFile(getVariableValue(args, getRunningPid()));
    }else if(!strcmp(instruction, "writeFile")) {
        char *argsCopy = strdup(args);
        char **tokens = str_split(argsCopy, ' ');
        char *filename = tokens[0];
        char *data = tokens[1];

        filename = getVariableValue(filename, getRunningPid());

        data = getVariableValue(data, getRunningPid());

        writeFile(filename, data);

    } else if (!strcmp(instruction, "printFromTo")) {
        char * argsCopy = strdup(args);
        char **tokens = str_split(argsCopy, ' ');

        // get the two numbers
        tokens[0] = getVariableValue(tokens[0], getRunningPid());
        tokens[1] = getVariableValue(tokens[1], getRunningPid());

        int x = atoi(tokens[0]);
        int y = atoi(tokens[1]);
        printFromTo(x, y);
    } else if (!strcmp(instruction, "assign")) {
        char *argsCopy = strdup(args);
        char **tokens = str_split(argsCopy, ' ');
        char *x = tokens[0];
        char *y = tokens[1];
        assign(x, y);
    } else if (!strcmp(instruction, "semWait")) {
        semWait(args);
    } else if (!strcmp(instruction, "semSignal")) {
        semSignal(args);
    }

}

int isInstruction(char *instruction)
{
    char *instructionSet[] = {"print", "readFile", "writeFile", "printFromTo", "assign", "semWait", "semSignal"};
    for (int i = 0; i < 7; i++)
    {
        if (!strcmp(instruction, instructionSet[i]))
        {
            return 1;
        }
    }
    return 0;
}

void printSemaphores()
{
    printf("Input Buffer Semaphore:\n");
    print_semaphore_queue(inputBufferSemaphore);
    printf("Output Buffer Semaphore:\n");
    print_semaphore_queue(outputBufferSemaphore);
    printf("File Buffer Semaphore:\n");
    print_semaphore_queue(fileBufferSemaphore);


}