#include <stddef.h>
#include <bits/types/FILE.h>
#include <dirent.h>
#include "insrtuction_set.h"
#include "memory.h"
#include "string.h"
#include "stdlib.h"
#include "assert.h"
#include "stdio.h"
#include "ctype.h"
#include "dispatcher.h"
#include "mlf_sched.h"
#include "queue.h"
#include "mlf_queue.h"
#include "STRING_OPERATIONS.h"
#include "pcb.h"




int memoryAddress = 0;
int pid = 1;



/**
 * Parses an array of tokens and generates an instruction based on the tokens.
 * The generated instruction is then written to the specified memory address.
 *
 * @param tokens An array of tokens to be parsed.
 * @param memoryAddress The memory address where the generated instruction will be written.
 */
void parseTokens(char **tokens)
{


    if (tokens)
    {



        char *name = trimwhitespace(*(tokens));

        char *value = trimwhitespace(*(tokens + 1));

        if(!strcmp(name, "writeFile") || !strcmp(name, "assign") || !strcmp(name, "printFromTo"))
        {
            strcat(value, " ");
            strcat(value, trimwhitespace(*(tokens + 2)));

            // concat rest of tokens to value and separate by space
            for (int i = 3; *(tokens + i); i++)
            {
                strcat(value, " ");
                strcat(value, trimwhitespace(*(tokens + i)));
            }

        }

        setMemoryWholeWord(memoryAddress, name, value);




    }
}

/**
 * Reads an assembly file and parses its tokens.
 *
 * @param programFilePath The path to the assembly file.
 */
void readProgramFile(char *programFilePath)
{
    printf("Reading file %s\n", programFilePath);
    // Open a file in read mode
    FILE *fptr = fopen(programFilePath, "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }



    char *line = NULL;

    size_t len = 0;

    ssize_t read;

    int lowerBound = memoryAddress;


    // Read the file line by line
    while ((read = getline(&line, &len, fptr)) != -1)
    {

        if (line[0] == '\n')
            continue;

        // split line by space
        char **tokens = str_split(line, ' ', NULL);

        // parse tokens
        parseTokens(tokens);

        memoryAddress++;
    }
    memoryAddress += 3;

    int upperBound = memoryAddress;

    char *lowerBoundStr = (char *) malloc(10);
    itoa(lowerBound, lowerBoundStr, 10);

    char *upperBoundStr = (char *) malloc(10);
    itoa(upperBound, upperBoundStr, 10);

    addNewPCB(upperBoundStr, lowerBoundStr);
    schedEnqueue(pid++);
    memoryAddress++;

}


void readProgramFiles(char *dirPath) {
    DIR *folder = opendir(dirPath);
    if (folder == NULL) {
        fprintf(stderr, "Unable to read folder %s\n", dirPath);
        return;
    }

    struct dirent *input;
    FILE *fileinput;
    int files = 0;

    // Use this file loop to go through each
    while ((input = readdir(folder)) != NULL) {
        char path[PATH_MAX];
        if (!strcmp(input->d_name, ".") || !strcmp(input->d_name, ".."))
            continue;

        if ((size_t) snprintf(path, sizeof path, "%s/%s", dirPath, input->d_name) >= sizeof path) {
            fprintf(stderr, "filename too long: %s/%s\n", dirPath, input->d_name);
            continue;
        }
        printf("Reading file %s\n", path);
        readProgramFile(path);

    }
}

int main(int argc, char **argv)
{


    // To run program run: ./main <program 1 path> <arrival time> <program 2 path> <arrival time> <program 3 path> <arrival time>

    if (argc < 7) {
        fprintf(stderr, "Usage: %s <program 1 path> <arrival time> <program 2 path> <arrival time> <program 3 path> <arrival time>\n", argv[0]);
        return 1;
    }

    int arrivalTimes[3];

    for (int i = 0; i < 3; i++) {
        arrivalTimes[i] = atoi(argv[2 * i + 2]);


    }

    int clock = 0;

    initMemory();
    initializeScheduler();
    initDispatcher();
    instruction_set_init();



    while ((argc - 1) / 2 > getNumberOfRemovedProcesses()) {

        clock++;

        for(int i = 0; i < 3; i++) {
            if (arrivalTimes[i] == clock) {
                readProgramFile(argv[2 * i + 1]);
            }
        }

        if (getRunningPid() == -1) {
            dispatch();
            if (getRunningPid() == -1) {
                continue;
            }
        }

//        printf("-------------------- START MEMORY AT CLOCK %d -------------------\n", clock);
//        printMemory();
//        printf("-------------------- END MEMORY AT CLOCK %d -------------------\n", clock);



        int pc = atoi(getPCBField("PC", getRunningPid()).value);

        MemoryWord currentInstruction = getMemoryWord(pc);
        incrementPC(getRunningPid());
        pc = atoi(getPCBField("PC", getRunningPid()).value);

        int upperBound = atoi(getPCBField("UPPER_BOUND", getRunningPid()).value);
        int lowerBound = atoi(getPCBField("LOWER_BOUND", getRunningPid()).value);

        MemoryWord word = getMemoryWord(pc);

        // if word is null or empty or out of bounds or not an instruction
        int previousPid = getRunningPid();
        int willBeRemoved = 0;
        if (word.name == NULL || word.value == NULL || pc > upperBound || pc < lowerBound || !isInstruction(word.name)) {
            willBeRemoved = 1;

        }else{
            willBeRemoved = 0;
        }

        executeInstruction(currentInstruction);
        if (willBeRemoved) {
            removeProcess(previousPid);
            setRunningPid(-1);
            continue;

        }
        if (getRunningPid() == -1) {
            continue;
        }









        int runningQuantum = getRunningQuantum();
        setRunningQuantum(runningQuantum + 1);

        if (++runningQuantum >= getCurrentQueueQuantum()){
            schedEnqueue(getRunningPid());
            setRunningPid(-1);
        }


    }

    printf("-------------------- FINAL MEMORY STATE -------------------\n");
    printMemory();
    printf("-------------------- END MEMORY FINAL STATE -------------------\n");



}









