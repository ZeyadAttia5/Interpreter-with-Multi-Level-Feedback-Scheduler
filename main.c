#define _GNU_SOURCE // for CPU_SET, CPU_ZERO macros
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>                                                                                                                                              


const long long int WORK_LOAD = 1000000000; // constant to simulate work load for each thread

int arrival_time[4]; // array to store the arrival time of each thread

int start_time[4]; // array to store the start time of each thread

int completion_time[4]; // array to store the completion time of each thread

int execution_time[4]; // array to store the execution time of each thread



// Function that the threads will run
void *thread_func(void *arg) {

    // Get the current time in microseconds

    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    unsigned long long int current_time_in_microseconds = current_time.tv_sec * 1000000 + current_time.tv_usec;

    long tid = (long)arg; // get the thread id

    start_time[tid] = current_time_in_microseconds; // store the start time of the thread

    

    long long int i = 0;

    

    

    printf("Thread %ld starting at timestamp %llu\n", tid, current_time_in_microseconds);
    
    
    
    for (i = 0; i < WORK_LOAD; i++)
    {
        if (i == WORK_LOAD / 2 || i == WORK_LOAD / 4 || i == 3 * WORK_LOAD / 4)
        {
            // Get the current time in microseconds
            gettimeofday(&current_time, NULL);
            current_time_in_microseconds = current_time.tv_sec * 1000000 + current_time.tv_usec;
            
            printf("Thread %ld working at timestamp %llu ...\n", tid, current_time_in_microseconds);
        }
    }

  
    printf("Thread %ld: End\n", tid);

    // Get the current time in microseconds
    gettimeofday(&current_time, NULL);

    current_time_in_microseconds = current_time.tv_sec * 1000000 + current_time.tv_usec;

    completion_time[tid] = current_time_in_microseconds; // store the completion time of the thread

    pthread_exit(NULL); // exit the thread
}


void printKPIs(){
    int turnaround_time[4]; // array to store the turnaround time of each thread
    int execution_time[4]; // array to store the execution time of each thread
    float unknown_metric[4]; // array to store the unknown metric of each thread
    float useful_cpu_work[4]; // array to store the useful cpu work of each thread

    for (int i = 0; i < 4; i++)
    {
        turnaround_time[i] = completion_time[i] - arrival_time[i]; // calculate the turnaround time of the thread
        printf("Thread %d: Turnaround time: %d\n", i, turnaround_time[i]);

        execution_time[i] = completion_time[i] - start_time[i]; // calculate the execution time of the thread
        printf("Thread %d: Execution time: %d\n", i, execution_time[i]);

        unknown_metric[i] = turnaround_time[i] /( (float) execution_time[i]); // calculate the unknown metric of the thread
        printf("Thread %d: Ri/ei: %f\n", i, unknown_metric[i]);

        useful_cpu_work[i] = (execution_time[i]) / ((float)(completion_time[i] - arrival_time[i])); // calculate the useful cpu work of the thread
        printf("Thread %d: Useful CPU work: %f\n", i, useful_cpu_work[i]);

        printf("--------------------------------------------\n");

    }
}



int main(int argc, char* argv[]){
    clock_t start_time = clock(); // start time of the program

    /*###########################################################################################################################
    ########################################## BEGIN SETTING CPU TO SINGLE CORE  ################################################
    #############################################################################################################################*/

    cpu_set_t set; 
    CPU_ZERO(&set);        // clear cpu mask
    CPU_SET(0, &set);      // set cpu 0
    sched_setaffinity(0, sizeof(cpu_set_t), &set);  // 0 is the calling process

    /*###########################################################################################################################
    ########################################## END SETTING CPU TO SINGLE CORE  ##################################################
    #############################################################################################################################*/


    /*###########################################################################################################################
    ########################################## BEGIN CREATION OF THREADS  #######################################################
    #############################################################################################################################*/
    
    int policy;
    // set the scheduling policy of the thread based on the command line argument
    if (argc == 2 && strcasecmp(argv[1], "fifo") == 0){
        policy = SCHED_FIFO;
    } else if (argc == 2 && strcasecmp(argv[1], "rr") == 0){
        policy = SCHED_RR;
    } else{
        policy = SCHED_OTHER;
    }
    
    int thread_priority = 99; // priority of the thread

    pthread_t threads[4];
    pthread_attr_t attrs[4];
    
    struct sched_param params[4];
    

    printf("Main: Start\n");

   

    for (int i = 0; i < 4; i++)
    {
        pthread_attr_init(&attrs[i]);

        // set inherit scheduler attribute so that the thread does not inherit the scheduler of the parent
        pthread_attr_setinheritsched(&attrs[i], PTHREAD_EXPLICIT_SCHED);

        // set the scheduling policy of the thread
        pthread_attr_setschedpolicy(&attrs[i], policy);

        // set the priority of the thread
        params[i].sched_priority = thread_priority;
        pthread_attr_setschedparam(&attrs[i], &params[i]);

        // get current timestamp

        struct timeval current_time;
        gettimeofday(&current_time, NULL);

        unsigned long long int current_time_in_microseconds = current_time.tv_sec * 1000000 + current_time.tv_usec;

        arrival_time[i] = current_time_in_microseconds; // store the arrival time of the thread

        
        pthread_create(&threads[i], &attrs[i], thread_func, (void *)(long)i);
        
          


    }

    /*###########################################################################################################################
    ########################################## END CREATION OF THREADS  #########################################################
    #############################################################################################################################*/


    /*###########################################################################################################################
    ########################################## BEGIN JOINING THREADS  ###########################################################
    #############################################################################################################################*/

    // Wait for the threads to finish
    for (int i = 0; i < 4; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Main: End\n");

    /*###########################################################################################################################
    ########################################## END JOINING THREADS  #############################################################
    #############################################################################################################################*/

    clock_t end_time = clock(); // end time of the program
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC; // time taken to run the program
    printf("Time taken: %lf\n", time_spent); 
    
    // print all KPIs

    printKPIs();


    return 0;

}





