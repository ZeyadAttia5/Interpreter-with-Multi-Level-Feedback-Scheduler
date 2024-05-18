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




int memoryAddress = 0;

/**
 * Splits a string into an array of substrings based on a delimiter.
 *
 * @param a_str The string to be split.
 * @param a_delim The delimiter character used to split the string.
 * @return A dynamically allocated array of strings representing the substrings.
 *         The last element of the array is set to NULL to indicate the end of the list.
 *         The caller is responsible for freeing the memory allocated for the array and its elements.
 */
char **str_split(char *a_str, const char a_delim)
{
    char **result = 0;
    size_t count = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char *) * count);

    if (result)
    {
        size_t idx = 0;
        char *token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


/**
 * Trims leading and trailing whitespace from a string.
 *
 * @param str The string to be trimmed.
 * @return A pointer to the trimmed string.
 */
char *trimwhitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}


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
        memoryAddress += 3;
    }
}

int main()
{






    readProgramFiles("../programs");

    setMemoryWholeWord(7, "a", "");
    setMemoryWholeWord(8, "b", "");

    setMemoryWholeWord(17, "a", "");
    setMemoryWholeWord(18, "b", "");
    setMemoryWholeWord(19, "input", "");


    addNewPCB("0", "10");
//    addNewPCB("10", "20");

    initializeScheduler();
    initDispatcher();
    schedEnqueue(1);



    dispatch();

    int pc = atoi(getPCBField("PC", getRunningPid()).value);

    MemoryWord currentInstruction;
    currentInstruction.name = "assign";
    currentInstruction.value = "a 5";

    executeInstruction(currentInstruction);

    pc++;

    char pcStr[10];
    sprintf(pcStr, "%d", pc);
    setPCBField("PC", getRunningPid(), pcStr);

    int runningQuantum = getRunningQuantum();
    setRunningQuantum(runningQuantum + 1);

    printMemory();

}









