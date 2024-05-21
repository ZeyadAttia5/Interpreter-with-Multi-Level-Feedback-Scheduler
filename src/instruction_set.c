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
#include "../include/mutex.h"
#include "pcb.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

mutex *inputBufferSemaphore, *outputBufferSemaphore, *fileBufferSemaphore;

void instruction_set_init() {

    create_mutex(1, "inputBuffer", &inputBufferSemaphore);
    create_mutex(1, "outputBuffer", &outputBufferSemaphore);
    create_mutex(1, "fileBuffer", &fileBufferSemaphore);

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
        printf("Process[%d]: Please enter a value for %s: ", getRunningPid(), x);
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
    if (!strcmp("input", filename))
    {
        char *input = malloc(100);
        // TODO: MUST ADD "Please enter a value for x" before scanf but doesn't work
        printf("Process[%d]: Please enter a value for %s: ", getRunningPid(), "filename");
        scanf("%[^\n]%*c", input);
        filename = input;
    }
    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Error opening file! EXITING PROCESS\n");
        removeProcess(getRunningPid());
        return;
    }



    fprintf(f, "%s", data);
    fclose(f);


}
char *readFile(char *filename)
{
    if (!strcmp("input", filename))
    {
        char *input = malloc(100);
        // TODO: MUST ADD "Please enter a value for x" before scanf but doesn't work
        printf("Process[%d]: Please enter a value for %s: ", getRunningPid(), filename);
        scanf("%[^\n]%*c", input);
        filename = input;
    }
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("Error opening file! EXITING PROCESS\n");
        removeProcess(getRunningPid());
        setRunningPid(-1); // to force the dispatcher to run the next process
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    string[fsize] = '\0'; // add null terminator to the end of the string
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
         if(try_wait_mutex(inputBufferSemaphore, pid) == 0){

             setRunningPid(-1); // to force the dispatcher to run the next process
             setRunningQuantum(0); // to force the dispatcher to run the next process

         }
     }else if (!strcmp(x, "userOutput")) {
         if (try_wait_mutex(outputBufferSemaphore, pid) == 0) {

             setRunningPid(-1); // to force the dispatcher to run the next process
             setRunningQuantum(0); // to force the dispatcher to run the next process

         }
     } else if (!strcmp(x, "file")) {
            if (try_wait_mutex(fileBufferSemaphore, pid) == 0) {

                setRunningPid(-1); // to force the dispatcher to run the next process
                setRunningQuantum(0); // to force the dispatcher to run the next process
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
        pid = signal_mutex(inputBufferSemaphore);
    } else if (!strcmp(x, "userOutput")) {
        pid = signal_mutex(outputBufferSemaphore);
    } else if (!strcmp(x, "file")) {
        pid = signal_mutex(fileBufferSemaphore);
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
        int len;
        char **tokens = str_split(argsCopy, ' ', &len);
        len--;
        *isNested = 1;
        MemoryWord nestedInstruction;

//        write readFile a b
//        write a readFile b
//        write redFile a readFile b
        char *filename1 = tokens[1];
        if (isVariableExists(filename1, getRunningPid()))
            filename1 = getVariableValue(filename1, getRunningPid());
        if (len == 4)
        {


            char *filename2 = tokens[3];


            if (isVariableExists(filename2, getRunningPid()))
                filename2 = getVariableValue(filename2, getRunningPid());

            char *file1_data = readFile(filename1);
            char *file2_data = readFile(filename2);

            nestedInstruction.name = instruction;

            nestedInstruction.value = strcat(file1_data, " ");
            nestedInstruction.value = strcat(nestedInstruction.value, file2_data);

        }else if (len == 3) {

            char *file_data;

            if(strstr(tokens[0], "readFile") != NULL) {
                file_data = readFile(filename1);
                nestedInstruction.value = strcat(file_data, " ");
                nestedInstruction.value = strcat(nestedInstruction.value, tokens[2]);

            }else if(strstr(tokens[1], "readFile") != NULL) {

                char *filename = tokens[2];
                if (isVariableExists(filename, getRunningPid()))
                    filename = getVariableValue(filename, getRunningPid());

                file_data = readFile(filename);
                nestedInstruction.value = strcat(tokens[0], " ");
                nestedInstruction.value = strcat(nestedInstruction.value, file_data);

            }

            if (file_data == NULL) {

                setRunningPid(-1);
                return;
            }

            *nestedOutput = file_data;


            nestedInstruction.name = instruction;


        }


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
        if (isVariableExists(args, getRunningPid()))
            print(getVariableValue(args, getRunningPid()));
        else
            print(args);
    }else if(!strcmp(instruction, "readFile")) {
        if (isVariableExists(args, getRunningPid()))
            readFile(getVariableValue(args, getRunningPid()));
        else
            readFile(args);

    }else if(!strcmp(instruction, "writeFile")) {
        char *argsCopy = strdup(args);
        char **tokens = str_split(argsCopy, ' ', NULL);
        char *filename = tokens[0];
        char *data = tokens[1];

        if (isVariableExists(filename, getRunningPid()))
            filename = getVariableValue(filename, getRunningPid());

        if (isVariableExists(data, getRunningPid()))
            data = getVariableValue(data, getRunningPid());

        writeFile(filename, data);

    } else if (!strcmp(instruction, "printFromTo")) {
        char * argsCopy = strdup(args);
        char **tokens = str_split(argsCopy, ' ', NULL);

        // get the two numbers
        if (isVariableExists(tokens[0], getRunningPid()))
            tokens[0] = getVariableValue(tokens[0], getRunningPid());
        if (isVariableExists(tokens[1], getRunningPid()))
            tokens[1] = getVariableValue(tokens[1], getRunningPid());

        int x = atoi(tokens[0]);
        int y = atoi(tokens[1]);
        printFromTo(x, y);
    } else if (!strcmp(instruction, "assign")) {
        char *argsCopy = strdup(args);
        char **tokens = str_split(argsCopy, ' ', NULL);
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
    print_mutex_queue(inputBufferSemaphore);
    printf("Output Buffer Semaphore:\n");
    print_mutex_queue(outputBufferSemaphore);
    printf("File Buffer Semaphore:\n");
    print_mutex_queue(fileBufferSemaphore);


}