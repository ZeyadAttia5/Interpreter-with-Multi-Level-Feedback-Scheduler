#ifndef PCB_H
#define PCB_H
enum STATE
{
    READY,
    BLOCKED,
    RUNNING
};

typedef struct PCB 
{
    int pid;
    enum STATE state;
    int priority;
    int pc;
    int memoryLowerBound;
    int memoryUpperBound;
} PCB;

#endif