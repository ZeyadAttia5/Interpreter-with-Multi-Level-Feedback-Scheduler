#ifndef OS_PROJECT_INSRTUCTION_SET_H
#define OS_PROJECT_INSRTUCTION_SET_H
#include "memory.h"


void instruction_set_init();
void print(char *x);
void assign(char *x, char *y);
void writeFile(char *filename, char *data);
char *readFile(char *filename);
void printFromTo(int x, int y);
void semWait(char *x);
void semSignal(char *x);
void executeInstruction(MemoryWord currentInstruction);
int isInstruction(char *instruction);
void printSemaphores();
#endif //OS_PROJECT_INSRTUCTION_SET_H
