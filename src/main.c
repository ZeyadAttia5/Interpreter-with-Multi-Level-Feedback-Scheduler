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
        char **tokens = str_split(line, ' ');

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

int main()
{

    /* TODO: Write new programs to test with */

    initMemory();
    initializeScheduler();
    initDispatcher();
    instruction_set_init();

    readProgramFile("../programs/Program_1.txt");
    readProgramFile("../programs/Program_3.txt");
    readProgramFile("../programs/Program_2.txt");













    dispatch();

    while (1) {


        if (getRunningPid() == -1) {
            break;
        }


        int pc = atoi(getPCBField("PC", getRunningPid()).value);

        MemoryWord currentInstruction = getMemoryWord(pc);

        executeInstruction(currentInstruction);

        pc++;

        char *pcStr = (char *) malloc(10);
        itoa(pc, pcStr, 10);



        setPCBField("PC", getRunningPid(), pcStr);

        int upperBound = atoi(getPCBField("UPPER_BOUND", getRunningPid()).value);
        int lowerBound = atoi(getPCBField("LOWER_BOUND", getRunningPid()).value);

        MemoryWord word = getMemoryWord(pc);

        // if word is null or empty or out of bounds or not an instruction

        if (word.name == NULL || word.value == NULL || pc > upperBound || pc < lowerBound || !isInstruction(word.name)) {
            // remove PCB from memory
//            removePCB(getRunningPid());

            setRunningPid(-1);
            setRunningQuantum(1);
            dispatch();

            continue;
        }



        int runningQuantum = getRunningQuantum();
        setRunningQuantum(runningQuantum + 1);

        if (runningQuantum == getCurrentQueueQuantum()){
            schedEnqueue(getRunningPid());
            setRunningPid(-1);
            setRunningQuantum(1);
            dispatch();
        }
    }

    printMemory();


}









