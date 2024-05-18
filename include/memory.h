#ifndef OS_PROJECT_MEMORY_H
#define OS_PROJECT_MEMORY_H
#define MEMORY_SIZE 60
#define PCB_SIZE 6


typedef struct MemoryWord
{
    char *name;
    char *value;
} MemoryWord;

typedef struct Memory
{
    MemoryWord memoryWords[MEMORY_SIZE];
} Memory;



void initMemory();
void setMemoryWholeWord(int address, char *name, char *value);
void setMemoryWordValue(int address, char *value);
MemoryWord getMemoryWord(int address);
MemoryWord getPCBField(char *fieldName, int pcbIndex);
void setPCBField(char *fieldName, int pcbIndex, char *value);
void addNewPCB(char *upperBound, char *lowerBound);
void printMemory();
char *getVariableValue(char *variableName, int runningPID);


#endif //OS_PROJECT_MEMORY_H
