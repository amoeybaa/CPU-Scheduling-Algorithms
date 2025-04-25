// Header file to implement CPU scheduling algorithms

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct process
{
    /*
        INTRODUCTION:
            Structure to represent a process in scheduling algorithms.

        VARIABLES:
            - pid (int):                Unique identifier for the process.
            - arrive_time (int):        Time at which the process arrives in the system.
            - burst_time (int):         CPU burst time required by the process.
            - turnaround_time (int):    Total time from arrival to completion (finish - arrival).
            - wait_time (int):          Total time the process waits in the ready queue.
            - priority (int):           Priority of the process (lower value = higher priority).
            - initial_burst (int):      Backup copy of the original burst time for calculations.
    */

    int pid, arrive_time, burst_time;
    int turnaround_time, wait_time;
    int priority, initial_burst;
}PROCESS;

typedef struct gantt
{
    /*
        INTRODUCTION:
            Structure to represent a single entry in a GANTT chart for process scheduling.

        VARIABLES:
            - pid (int):            Process ID executed during this time slot (-1 for IDLE).
            - start_time (int):     Starting time of this GANTT chart entry.
            - finish_time (int):    Ending time of this GANTT chart entry.
            - next (struct gantt*): Pointer to the next GANTT chart entry (linked list structure).
    */

    int pid, start_time, finish_time;
    struct gantt *next;
}GANTT;

PROCESS* getProcesses(int);
PROCESS* getProcessesPriority(int);
int getProcessesFile(char *, PROCESS **);
int getProcessesFilePriority(char *, PROCESS **);
void displayProcesses(PROCESS *, int);
void destroyProcesses(PROCESS *);

// *** Non-Preemptive Scheduling Algorithms ***
GANTT* getGanttFCFS(PROCESS *, int);
GANTT* getGanttSJF(PROCESS *, int);
GANTT* getGanttHRRN(PROCESS *, int);
GANTT* getGanttPriority(PROCESS *, int);
GANTT* getGanttLJF(PROCESS *, int);

// *** Preemptive Scheduling Algorithms ***
GANTT* getGanttSRTF(PROCESS *, int);
GANTT* getGanttPriorityPreemptive(PROCESS *, int);
GANTT* getGanttLRTF(PROCESS *, int);

void displayGantt(GANTT *);
void destroyGantt(GANTT *);

PROCESS* getProcesses(int n) {
    /*
        INTRODUCTION:
            Reads process information from user input interactively.

        INPUT PARAMETERS:
            - n (int): number of processes to read.

        OUTPUT PARAMETERS:
            - None.

        RETURN VALUES:
            - (PROCESS*): pointer to dynamically allocated array of PROCESS structs.

        APPROACH:
            - Validate n, then allocate memory for n processes.
            - Loop n times, read arrival and burst times from user and validate the times.
            - Return pointer to the array.
    */

    if(n < 1) {
        printf("\nInvalid number of processes!\n");
        return NULL;
    }

    PROCESS *process = (PROCESS *)malloc(n*sizeof(PROCESS));

    for(int i = 0; i < n; ++i) {
        process[i].pid = i;
        process[i].priority = -1;
        printf("\nProcess %d:-", i);
        int check = -1;

        do
        {
            printf("\n\tArrival time: ");           // accept arrival time
            scanf("%d", &check);
            if(check < 0)
                printf("\nInvalid value! Enter again!\n");
        } while(check < 0);
        process[i].arrive_time = check;

        check = -1;
        do
        {
            printf("\n\tBurst time: ");             // accept burst time
            scanf("%d", &check);
            if(check < 0)
                printf("\nInvalid value! Enter again!\n");
        } while(check < 0);
        process[i].burst_time = check;
        process[i].initial_burst = check;
    }
    return process;
}

PROCESS* getProcessesPriority(int n) {
    /*
        INTRODUCTION:
            Reads process information with priority values from user input.

        INPUT PARAMETERS:
            - n (int): number of processes to read.

        OUTPUT PARAMETERS:
            - None.

        RETURN VALUES:
            - (PROCESS*): pointer to dynamically allocated array of PROCESS structs.

        APPROACH:
            - Validate n, then allocate memory for n processes.
            - Loop n times, read arrival, burst, and priority values and validate them.
            - Return pointer to the array.
    */

    if(n < 1) {
        printf("\nInvalid number of processes!\n");
        return NULL;
    }

    PROCESS *process = (PROCESS *)malloc(n*sizeof(PROCESS));

    for(int i = 0; i < n; ++i) {
        process[i].pid = i;
        printf("\nProcess %d:-", i);
        int check = -1;

        do
        {
            printf("\n\tArrival time: ");           // accept arrival time
            scanf("%d", &check);
            if(check < 0)
                printf("\nInvalid value! Enter again!\n");
        } while(check < 0);
        process[i].arrive_time = check;

        do
        {
            printf("\n\tBurst time: ");             // accept burst time
            scanf("%d", &check);
            if(check < 0)
                printf("\nInvalid value! Enter again!\n");
        } while(check < 0);
        process[i].burst_time = check;
        process[i].initial_burst = check;

        do
        {
            printf("\n\tPriority: ");             // accept burst time
            scanf("%d", &check);
            if(check < 0)
                printf("\nInvalid value! Enter again!\n");
        } while(check < 0);
        process[i].priority = check;
    }
    return process;
}

int getProcessesFile(char *fileName, PROCESS **process) {
    /*
        INTRODUCTION:
            Reads process information from a file.

        INPUT PARAMETERS:
            - fileName (char*): path to file / file name.
            - process (PROCESS**): pointer to array of PROCESS structs (to be allocated).

        OUTPUT PARAMETERS:
            - Allocates and fills *p with process data.

        RETURN VALUES:
            - (int): number of processes read from the file.

        APPROACH:
            - Open the file.
            - Read number of processes (n) and validate it.
            - Allocate memory and read arrival and burst times and validate the times.
            - Close file.
            - Return number of processes, return -1 if any error.
    */

    FILE *fptr = fopen(fileName, "r");
    if(!fptr) {
        printf("\nFailed to open the file!\n");
        *process = NULL;
        return -1;
    }

    int n = 0, check_at, check_bt;

    fscanf(fptr, "%d", &n);
    if(n < 1) {
        printf("\nInvalid number of processes!\n");
        fclose(fptr);
        *process = NULL;
        return -1;
    }

    if(*(process)) {
        free(*process);
        *process = NULL;
    }
    *process = (PROCESS *)malloc(n*sizeof(PROCESS));

    for (int i = 0; i < n; ++i) {
        check_at = check_bt = -1;

        if(fscanf(fptr, "%d %d", &check_at, &check_bt) != 2) {
            printf("\nIncomplete data in the file at process %d!\n", i);
            fclose(fptr);
            *process = NULL;
            return -1;
        }
        if(check_at < 0 || check_bt < 0) {
            printf("\nInvalid value for arrival/burst time at process %d!\n", i);
            free(*(process));
            *process = NULL;
            return -1;
        }
        else {
            (*process)[i].pid = i;
            (*process)[i].arrive_time = check_at;
            (*process)[i].burst_time = check_bt;
            (*process)[i].initial_burst = check_bt;
            (*process)[i].priority = -1;
        }
    }
    fclose(fptr);
    return n;
}

int getProcessesFilePriority(char *fileName, PROCESS **process) {
    /*
        INTRODUCTION:
            Reads process information with priorities from a file.

        INPUT PARAMETERS:
            - fileName (char*): path to file.
            - process (PROCESS**): pointer to array of PROCESS structs (to be allocated).

        OUTPUT PARAMETERS:
            - Allocates and fills *p with process data including priorities.

        RETURN VALUES:
            - (int): number of processes read from file.

        APPROACH:
            - Open file.
            - Read number of processes (n) and validate it.
            - Allocate memory and read arrival, burst, priority values and validate them.
            - Close file.
            - Return number of processes, return -1 if any error.
    */

    FILE *fptr = fopen(fileName, "r");
    if(!fptr) {
        printf("\nFailed to open the file!\n");
        *process = NULL;
        return -1;
    }

    int n = 0, check_at, check_bt, check_p;

    fscanf(fptr, "%d", &n);
    if(n < 1) {
        printf("\nInvalid number of processes!\n");
        fclose(fptr);
        *process = NULL;
        return -1;
    }

    if(*(process)) {
        free(*process);
        *process = NULL;
    }
    *process = (PROCESS *)malloc(n*sizeof(PROCESS));

    for (int i = 0; i < n; ++i) {
        check_at = check_bt = check_p = -1;

        if(fscanf(fptr, "%d %d %d", &check_at, &check_bt, &check_p) != 3) {
            printf("\nIncomplete data in the file at process %d!\n", i);
            fclose(fptr);
            *process = NULL;
            return -1;
        }
        if(check_at < 0 || check_bt < 0 || check_p < 0) {
            printf("\nInvalid value for arrival/burst time/priority at process %d!\n", i);
            free(*(process));
            *process = NULL;
            return -1;
        }
        else {
            (*process)[i].pid = i;
            (*process)[i].arrive_time = check_at;
            (*process)[i].burst_time = check_bt;
            (*process)[i].initial_burst = check_bt;
            (*process)[i].priority = check_p;
        }
    }
    fclose(fptr);
    return n;
}

void displayProcesses(PROCESS *process, int n) {
    /*
        INTRODUCTION:
            Displays process table information (Wait time and Turnaround time)

        INPUT PARAMETERS:
            - process (PROCESS*): pointer to process array.
            - n (int): number of processes.

        OUTPUT PARAMETERS:
            - Prints process details to console.

        RETURN VALUES:
            - None.

        APPROACH:
            - Print header.
            - Loop through processes and print each one's details.
    */

    float waitSum = 0, tatSum = 0;
    printf("\n\nWaiting time and Turn-around time:-");
    if(process[0].priority == -1) {
        printf("\nPID\tWait\tTAT\n------------------------");
        for(int i = 0; i < n; i++)
        {
            printf("\nP%d\t%d\t%d", process[i].pid, process[i].wait_time, process[i].turnaround_time);
            waitSum += process[i].wait_time;
            tatSum += process[i].turnaround_time;
        }
    }
    else {
        printf("\nPrty\tPID\tWait\tTAT\n--------------------------------");
        for(int i = 0; i < n; i++)
        {
            printf("\n%d\tP%d\t%d\t%d", process[i].priority, process[i].pid, process[i].wait_time, process[i].turnaround_time);
            waitSum += process[i].wait_time;
            tatSum += process[i].turnaround_time;
        }
    }

    printf("\n\nAverage Waiting time: %.2f", waitSum/n);
    printf("\nAverage Turn-around time: %.2f", tatSum/n);
    printf("\n----------------------------------------------------------------\n");
}

void destroyProcesses(PROCESS *process) {
    /*
        INTRODUCTION:
            Frees allocated memory for process array.

        INPUT PARAMETERS:
            - process (PROCESS*): pointer to process array.

        OUTPUT PARAMETERS:
            - Frees dynamically allocated memory.

        RETURN VALUES:
            - None.

        APPROACH:
            - Use free() on process.
    */

    if(process) free(process);
    process = NULL;
}

GANTT* getGanttFCFS(PROCESS *process, int n) {
    
/*
        INTRODUCTION:
            Simulates First-Come First-Serve (FCFS) scheduling.

        INPUT PARAMETERS:
            - process (PROCESS*): process array.
            - n (int): number of processes.

        OUTPUT PARAMETERS:
            - Constructs GANTT chart linked list.

        RETURN VALUES:
            - (GANTT*): pointer to GANTT chart head.

        APPROACH:
            - Sort processes by arrival time.
            - Execute processes in order, updating GANTT linked list.
    */

    if(n < 1) {
        printf("\nInvalid number of Processes!\n");
        return NULL;
    }

    if(process) {
        for(int i = 0; i < n-1; i++)                // sort according to arrival time
        {
            for(int j = i+1; j < n; j++)
            {
                if(process[i].arrive_time > process[j].arrive_time)
                {
                    PROCESS t = process[i];
                    process[i] = process[j];
                    process[j] = t;
                }
            }
        }
    }
    else {
        printf("\nFailed to read the processes!\n");
        return NULL;
    }

    GANTT *ghead = NULL, *gnew = NULL, *gcurr = NULL;

    if(process[0].arrive_time != 0)
    {
        ghead = (GANTT *)malloc(sizeof(GANTT));
        ghead->pid = -1;                        // -1 for IDLE
        ghead->start_time = 0;
        ghead->finish_time = process[0].arrive_time;
        ghead->next = NULL;
        gcurr = ghead;
    }

    for(int i = 0; i < n; i++)
    {
        gnew = (GANTT *)malloc(sizeof(GANTT));

        if(!ghead)
        {
            gnew->pid = process[i].pid;
            gnew->start_time = 0;
            gnew->finish_time = process[i].burst_time;
            gnew->next = NULL;
            ghead = gcurr = gnew;

            process[i].wait_time = 0;
            process[i].turnaround_time = gcurr->finish_time - process[i].arrive_time;
        }
        else
        {
            if(process[i].arrive_time <= gcurr->finish_time)
            {
                gnew->pid = process[i].pid;
                gnew->start_time = gcurr->finish_time;
                gnew->finish_time = gnew->start_time + process[i].burst_time;
                gnew->next = NULL;
                gcurr->next = gnew;
                gcurr = gnew;

                process[i].wait_time = gcurr->start_time - process[i].arrive_time;
                process[i].turnaround_time = gcurr->finish_time - process[i].arrive_time;
            }
            else
            {
                gnew->pid = -1;                 // System is IDLE
                gnew->start_time = gcurr->finish_time;
                gnew->finish_time = process[i].arrive_time;
                gnew->next = NULL;
                gcurr->next = gnew;
                gcurr = gnew;
                i--;
            }
        }        
    }
    return ghead;
}

GANTT* getGanttSJF(PROCESS *process, int n) {
    /*
        INTRODUCTION:
            Simulates Shortest Job First (SJF) non-preemptive scheduling.

        INPUT PARAMETERS:
            - process (PROCESS*): process array.
            - n (int): number of processes.

        OUTPUT PARAMETERS:
            - Constructs GANTT chart linked list.

        RETURN VALUES:
            - (GANTT*): pointer to GANTT chart head.

        APPROACH:
            - Sort processes by arrival time, or on burst time if arrival times are same.
            - Select shortest available job at each time point.
            - Update GANTT chart and process stats.
    */

    if(n < 1) {
        printf("\nInvalid number of Processes!\n");
        return NULL;
    }

    if(process) {
        for(int i = 0; i < n-1; i++)                // sort according to arrival time
        {
            for(int j = i+1; j < n; j++)
            {
                if(process[i].arrive_time > process[j].arrive_time)
                {
                    PROCESS t = process[i];
                    process[i] = process[j];
                    process[j] = t;
                    continue;
                }
                if(process[i].arrive_time == process[j].arrive_time) {          // if same arrival time, sort on burst time
                    if(process[i].burst_time > process[j].burst_time) {
                        PROCESS t = process[i];
                        process[i] = process[j];
                        process[j] = t;
                    }
                }
            }
        }
    }
    else {
        printf("\nFailed to read the processes!\n");
        return NULL;
    }

    GANTT *ghead = NULL, *gnew = NULL, *gcurr = NULL;

    if(process[0].arrive_time != 0)
    {
        ghead = (GANTT *)malloc(sizeof(GANTT));
        ghead->pid = -1;                        // -1 for IDLE
        ghead->start_time = 0;
        ghead->finish_time = process[0].arrive_time;
        ghead->next = NULL;
        gcurr = ghead;
    }

    for(int i = 0; i < n; ++i) {
        int mini = -1;

        gnew = (GANTT *)malloc(sizeof(GANTT));

        if(!ghead) {
            gnew->pid = process[i].pid;
            gnew->start_time = 0;
            gnew->finish_time = process[i].burst_time;
            gnew->next = NULL;
            ghead = gcurr = gnew;

            process[i].wait_time = 0;
            process[i].turnaround_time = gcurr->finish_time - process[i].arrive_time;
        }
        else {
            if(process[i].arrive_time <= gcurr->finish_time) {
                int j = i+1;
                mini = i;

                while(j < n && process[j].arrive_time <= gcurr->finish_time) j++;

                for(int k = i+1; k < j; ++k) {
                    if(process[k].burst_time < process[mini].burst_time) {mini = k;}
                }
                if(mini != i) {
                    PROCESS temp = process[mini];
                    for (int k = mini; k > i; --k) {
                        process[k] = process[k - 1];
                    }
                    process[i] = temp;
                }

                gnew->pid = process[i].pid;
                gnew->start_time = gcurr->finish_time;
                gnew->finish_time = gnew->start_time + process[i].burst_time;
                gnew->next = NULL;
                gcurr->next = gnew;
                gcurr = gnew;

                process[i].wait_time = gcurr->start_time - process[i].arrive_time;
                process[i].turnaround_time = gcurr->finish_time - process[i].arrive_time;

            }
            else {
                gnew->pid = -1;                 // System is IDLE
                gnew->start_time = gcurr->finish_time;
                gnew->finish_time = process[i].arrive_time;
                gnew->next = NULL;
                gcurr->next = gnew;
                gcurr = gnew;
                i--;
            }
        }
    }
    return ghead;
}

GANTT* getGanttHRRN(PROCESS *process, int n) {
    /*
        INTRODUCTION:
            Simulates Highest Response Ratio Next (HRRN) scheduling.

        INPUT PARAMETERS:
            - process (PROCESS*): process array.
            - n (int): number of processes.

        OUTPUT PARAMETERS:
            - Constructs GANTT chart linked list.

        RETURN VALUES:
            - (GANTT*): pointer to GANTT chart head.

        APPROACH:
            - Sort processes on arrival time, or on burst time if arrival times are same.
            - At each step, pick process with highest response ratio.
            - Update GANTT chart and process stats.
    */

    if(n < 1) {
        printf("\nInvalid number of Processes!\n");
        return NULL;
    }

    if(process) {
        for(int i = 0; i < n-1; i++)                // sort according to arrival time
        {
            for(int j = i+1; j < n; j++)
            {
                if(process[i].arrive_time > process[j].arrive_time)
                {
                    PROCESS t = process[i];
                    process[i] = process[j];
                    process[j] = t;
                    continue;
                }

                if(process[i].arrive_time == process[j].arrive_time) {          // if same arrival time, sort on burst time
                    if(process[i].burst_time > process[j].burst_time) {
                        PROCESS t = process[i];
                        process[i] = process[j];
                        process[j] = t;
                    }
                }
            }
        }
    }
    else {
        printf("\nFailed to read the processes!\n");
        return NULL;
    }

    GANTT *ghead = NULL, *gnew = NULL, *gcurr = NULL;

    if(process[0].arrive_time != 0)
    {
        ghead = (GANTT *)malloc(sizeof(GANTT));
        ghead->pid = -1;                        // -1 for IDLE
        ghead->start_time = 0;
        ghead->finish_time = process[0].arrive_time;
        ghead->next = NULL;
        gcurr = ghead;
    }

    for(int i = 0; i < n; ++i) {
        int mini = -1;
        float max_hrrn = -1.0;

        gnew = (GANTT *)malloc(sizeof(GANTT));

        if(!ghead) {
            gnew->pid = process[i].pid;
            gnew->start_time = 0;
            gnew->finish_time = process[i].burst_time;
            gnew->next = NULL;
            ghead = gcurr = gnew;

            process[i].wait_time = 0;
            process[i].turnaround_time = gcurr->finish_time - process[i].arrive_time;
        }

        else {
            if(process[i].arrive_time <= gcurr->finish_time) {
                mini = i;
                int j = i+1;
                while(j < n && process[j].arrive_time <= gcurr->finish_time) ++j;

                for(int k = i; k < j; ++k) {
                    float wait = (float)(gcurr->finish_time - process[k].arrive_time);
                    float hrrn = 1+ (float)(wait / process[k].burst_time);
                    if(hrrn > max_hrrn) {
                        mini = k;
                        max_hrrn = hrrn;
                    }
                }

                if(mini != i) {
                    PROCESS temp = process[mini];
                    for (int k = mini; k > i; --k) {
                        process[k] = process[k - 1];
                    }
                    process[i] = temp;
                }

                gnew->pid = process[i].pid;
                gnew->start_time = gcurr->finish_time;
                gnew->finish_time = gnew->start_time + process[i].burst_time;
                gnew->next = NULL;
                gcurr->next = gnew;
                gcurr = gnew;

                process[i].wait_time = gcurr->start_time - process[i].arrive_time;
                process[i].turnaround_time = gcurr->finish_time - process[i].arrive_time;
            }

            else {
                gnew->pid = -1;                 // System is IDLE
                gnew->start_time = gcurr->finish_time;
                gnew->finish_time = process[i].arrive_time;
                gnew->next = NULL;
                gcurr->next = gnew;
                gcurr = gnew;
                i--;
            }
        }
    }
    return ghead;
}

GANTT* getGanttPriority(PROCESS *process, int n) {
    /*
        INTRODUCTION:
            Simulates non-preemptive priority scheduling.

        INPUT PARAMETERS:
            - process (PROCESS*): process array.
            - n (int): number of processes.

        OUTPUT PARAMETERS:
            - Constructs GANTT chart linked list.

        RETURN VALUES:
            - (GANTT*): pointer to GANTT chart head.

        APPROACH:
            - Sort processes on arrival time, or on priority if arrival times are same.
            - Pick available process with highest priority.
            - Execute, update GANTT chart.
    */

    if(n < 1) {
        printf("\nInvalid number of Processes!\n");
        return NULL;
    }

    if(process) {
        for(int i = 0; i < n-1; i++)                // sort according to arrival time
        {
            for(int j = i+1; j < n; j++)
            {
                if(process[i].arrive_time > process[j].arrive_time)
                {
                    PROCESS t = process[i];
                    process[i] = process[j];
                    process[j] = t;
                    continue;
                }

                if(process[i].arrive_time == process[j].arrive_time) {          // if same arrival time, sort on priority
                    if(process[i].priority > process[j].priority) {
                        PROCESS t = process[i];
                        process[i] = process[j];
                        process[j] = t;
                    }
                }
            }
        }
    }
    else {
        printf("\nFailed to read the processes!\n");
        return NULL;
    }


    GANTT *ghead = NULL, *gnew = NULL, *gcurr = NULL;

    if(process[0].arrive_time != 0)
    {
        ghead = (GANTT *)malloc(sizeof(GANTT));
        ghead->pid = -1;                        // -1 for IDLE
        ghead->start_time = 0;
        ghead->finish_time = process[0].arrive_time;
        ghead->next = NULL;
        gcurr = ghead;
    }

    for(int i = 0; i < n; ++i) {
        int mini = -1;

        gnew = (GANTT *)malloc(sizeof(GANTT));

        if(!ghead) {
            gnew->pid = process[i].pid;
            gnew->start_time = 0;
            gnew->finish_time = process[i].burst_time;
            gnew->next = NULL;
            ghead = gcurr = gnew;

            process[i].wait_time = 0;
            process[i].turnaround_time = gcurr->finish_time - process[i].arrive_time;
        }
        else {
            if(process[i].arrive_time <= gcurr->finish_time) {
                mini = i;
                int j = i+1;
                while(j < n && process[j].arrive_time <= gcurr->finish_time) ++j;

                for(int k = i; k < j; ++k) {
                    if(process[mini].priority > process[k].priority) {
                        mini = k;
                    }
                }

                if(mini != i) {
                    PROCESS temp = process[mini];
                    for (int k = mini; k > i; --k) {
                        process[k] = process[k - 1];
                    }
                    process[i] = temp;
                }

                gnew->pid = process[i].pid;
                gnew->start_time = gcurr->finish_time;
                gnew->finish_time = gnew->start_time + process[i].burst_time;
                gnew->next = NULL;
                gcurr->next = gnew;
                gcurr = gnew;

                process[i].wait_time = gcurr->start_time - process[i].arrive_time;
                process[i].turnaround_time = gcurr->finish_time - process[i].arrive_time;
            }

            else {
                gnew->pid = -1;                 // System is IDLE
                gnew->start_time = gcurr->finish_time;
                gnew->finish_time = process[i].arrive_time;
                gnew->next = NULL;
                gcurr->next = gnew;
                gcurr = gnew;
                i--;
            }
        }
    }
    return ghead;
}

GANTT* getGanttLJF(PROCESS *process, int n) {
    /*
        INTRODUCTION:
            Simulates Longest Job First (LJF) non-preemptive scheduling.

        INPUT PARAMETERS:
            - process (PROCESS*): process array.
            - n (int): number of processes.

        OUTPUT PARAMETERS:
            - Constructs GANTT chart linked list.

        RETURN VALUES:
            - (GANTT*): pointer to GANTT chart head.

        APPROACH:
            - Sort processes on arrival time, or on burst time in descending order if arrival times are same.
            - At each step, select longest available job.
            - Update GANTT chart and stats.
    */
    
    if(n < 1) {
        printf("\nInvalid number of Processes!\n");
        return NULL;
    }

    if(process) {
        for(int i = 0; i < n-1; i++)                // sort according to arrival time
        {
            for(int j = i+1; j < n; j++)
            {
                if(process[i].arrive_time > process[j].arrive_time)
                {
                    PROCESS t = process[i];
                    process[i] = process[j];
                    process[j] = t;
                    continue;
                }
                if(process[i].arrive_time == process[j].arrive_time) {          // if same arrival time, sort on burst time descending
                    if(process[i].burst_time < process[j].burst_time) {
                        PROCESS t = process[i];
                        process[i] = process[j];
                        process[j] = t;
                    }
                }
            }
        }
    }
    else {
        printf("\nFailed to read the processes!\n");
        return NULL;
    }

    GANTT *ghead = NULL, *gnew = NULL, *gcurr = NULL;

    if(process[0].arrive_time != 0)
    {
        ghead = (GANTT *)malloc(sizeof(GANTT));
        ghead->pid = -1;                        // -1 for IDLE
        ghead->start_time = 0;
        ghead->finish_time = process[0].arrive_time;
        ghead->next = NULL;
        gcurr = ghead;
    }

    for(int i = 0; i < n; ++i) {
        int maxi = -1;

        gnew = (GANTT *)malloc(sizeof(GANTT));

        if(!ghead) {
            gnew->pid = process[i].pid;
            gnew->start_time = 0;
            gnew->finish_time = process[i].burst_time;
            gnew->next = NULL;
            ghead = gcurr = gnew;

            process[i].wait_time = 0;
            process[i].turnaround_time = gcurr->finish_time - process[i].arrive_time;
        }
        else {
            if(process[i].arrive_time <= gcurr->finish_time) {
                int j = i+1;
                maxi = i;

                while(j < n && process[j].arrive_time <= gcurr->finish_time) j++;

                for(int k = i+1; k < j; ++k) {
                    if(process[k].burst_time > process[maxi].burst_time) {maxi = k;}
                }
                if(maxi != i) {
                    PROCESS temp = process[maxi];
                    for (int k = maxi; k > i; --k) {
                        process[k] = process[k - 1];
                    }
                    process[i] = temp;
                }

                gnew->pid = process[i].pid;
                gnew->start_time = gcurr->finish_time;
                gnew->finish_time = gnew->start_time + process[i].burst_time;
                gnew->next = NULL;
                gcurr->next = gnew;
                gcurr = gnew;

                process[i].wait_time = gcurr->start_time - process[i].arrive_time;
                process[i].turnaround_time = gcurr->finish_time - process[i].arrive_time;

            }
            else {
                gnew->pid = -1;                 // System is IDLE
                gnew->start_time = gcurr->finish_time;
                gnew->finish_time = process[i].arrive_time;
                gnew->next = NULL;
                gcurr->next = gnew;
                gcurr = gnew;
                i--;
            }
        }
    }
    return ghead;
}

GANTT* getGanttSRTF(PROCESS *process, int n) {
    /*
        INTRODUCTION:
            Simulates Shortest Remaining Time First (SRTF) preemptive scheduling.

        INPUT PARAMETERS:
            - process (PROCESS*): process array.
            - n (int): number of processes.

        OUTPUT PARAMETERS:
            - Constructs GANTT chart linked list.

        RETURN VALUES:
            - (GANTT*): pointer to GANTT chart head.

        APPROACH:
            - Sort processes on arrival time, or on burst time if arrival times are same.
            - At each time unit, run process with shortest remaining time.
            - Preempt if necessary.
            - Update GANTT chart and process stats.
    */

    if(n < 1) {
        printf("\nInvalid number of Processes!\n");
        return NULL;
    }

    if(process) {
        for(int i = 0; i < n-1; i++)                // sort according to arrival time
        {
            for(int j = i+1; j < n; j++)
            {
                if(process[i].arrive_time > process[j].arrive_time)
                {
                    PROCESS t = process[i];
                    process[i] = process[j];
                    process[j] = t;
                    continue;
                }
                if(process[i].arrive_time == process[j].arrive_time) {          // if same arrival time, sort on burst time
                    if(process[i].burst_time > process[j].burst_time) {
                        PROCESS t = process[i];
                        process[i] = process[j];
                        process[j] = t;
                    }
                }
            }
        }
    }
    else {
        printf("\nFailed to read the processes!\n");
        return NULL;
    }

    GANTT *ghead = NULL, *gnew = NULL, *gcurr = NULL;
    int completed = 0, curr_time = 0;

    while (completed < n) {
        int min_idx = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (process[i].arrive_time <= curr_time && process[i].burst_time > 0) {
                if (process[i].burst_time < min_burst) {
                    min_burst = process[i].burst_time;
                    min_idx = i;
                }
            }
        }

        GANTT *gnew = (GANTT*)malloc(sizeof(GANTT));

        if(min_idx == -1) {            // system is IDLE
            gnew->pid = -1;
            gnew->start_time = curr_time;
            gnew->finish_time = curr_time + 1;
            gnew->next = NULL;
        }
        else {
            gnew->pid = process[min_idx].pid;
            gnew->start_time = curr_time;
            gnew->finish_time = curr_time + 1;
            gnew->next = NULL;

            process[min_idx].burst_time--;

            if (process[min_idx].burst_time == 0) {
                completed++;
                process[min_idx].turnaround_time = gnew->finish_time - process[min_idx].arrive_time;
                process[min_idx].wait_time = process[min_idx].turnaround_time - process[min_idx].initial_burst;
            }
        }

        if(gcurr) {
            if(min_idx == -1 && gcurr->pid == -1) {
                gcurr->finish_time = gnew->finish_time;
                free(gnew);
                gnew = NULL;
            }
            if(min_idx != -1 && (gcurr->pid == process[min_idx].pid)) {
                gcurr->finish_time = gnew->finish_time;
                free(gnew);
                gnew = NULL;
            }
        }

        if(!ghead) {
            ghead = gcurr = gnew;
        }
        else {
            if(gnew) {
                gcurr->next = gnew;
                gcurr = gnew;
            }
        }
        curr_time++;
    }

    return ghead;
}

GANTT* getGanttPriorityPreemptive(PROCESS *process, int n) {
    /*
        INTRODUCTION:
            Simulates Priority Preemptive scheduling.

        INPUT PARAMETERS:
            - process (PROCESS*): process array.
            - n (int): number of processes.

        OUTPUT PARAMETERS:
            - Constructs GANTT chart linked list.

        RETURN VALUES:
            - (GANTT*): pointer to GANTT chart head.

        APPROACH:
            - Sort processes on arrival times, or on priority if arrival times are same.
            - At each time unit, pick highest-priority (lowest value) process.
            - Preempt if a higher-priority process arrives.
            - Update GANTT chart and process stats.
    */

    if(n < 1) {
        printf("\nInvalid number of Processes!\n");
        return NULL;
    }

    if(process) {
        for(int i = 0; i < n-1; i++)                // sort according to arrival time
        {
            for(int j = i+1; j < n; j++)
            {
                if(process[i].arrive_time > process[j].arrive_time)
                {
                    PROCESS t = process[i];
                    process[i] = process[j];
                    process[j] = t;
                    continue;
                }

                if(process[i].arrive_time == process[j].arrive_time) {          // if same arrival time, sort on priority
                    if(process[i].priority > process[j].priority) {
                        PROCESS t = process[i];
                        process[i] = process[j];
                        process[j] = t;
                    }
                }
            }
        }
    }
    else {
        printf("\nFailed to read the processes!\n");
        return NULL;
    }

    GANTT *ghead = NULL, *gnew = NULL, *gcurr = NULL;
    int completed = 0, curr_time = 0;

    while (completed < n) {
        int min_idx = -1;
        int min_priority = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (process[i].arrive_time <= curr_time && process[i].burst_time > 0) {
                if (process[i].priority < min_priority) {
                    min_priority = process[i].priority;
                    min_idx = i;
                }
            }
        }

        GANTT *gnew = (GANTT*)malloc(sizeof(GANTT));

        if(min_idx == -1) {            // system is IDLE
            gnew->pid = -1;
            gnew->start_time = curr_time;
            gnew->finish_time = curr_time + 1;
            gnew->next = NULL;
        }
        else {
            gnew->pid = process[min_idx].pid;
            gnew->start_time = curr_time;
            gnew->finish_time = curr_time + 1;
            gnew->next = NULL;

            process[min_idx].burst_time--;

            if (process[min_idx].burst_time == 0) {
                completed++;
                process[min_idx].turnaround_time = gnew->finish_time - process[min_idx].arrive_time;
                process[min_idx].wait_time = process[min_idx].turnaround_time - process[min_idx].initial_burst;
            }
        }

        if(gcurr) {
            if(min_idx == -1 && gcurr->pid == -1) {
                gcurr->finish_time = gnew->finish_time;
                free(gnew);
                gnew = NULL;
            }
            if(min_idx != -1 && (gcurr->pid == process[min_idx].pid)) {
                gcurr->finish_time = gnew->finish_time;
                free(gnew);
                gnew = NULL;
            }
        }

        if(!ghead) {
            ghead = gcurr = gnew;
        }
        else {
            if(gnew) {
                gcurr->next = gnew;
                gcurr = gnew;
            }
        }
        curr_time++;
    }

    return ghead;
}

GANTT* getGanttLRTF(PROCESS *process, int n) {
    /*
        INTRODUCTION:
            Simulates Longest Remaining Time First (LRTF) preemptive scheduling.

        INPUT PARAMETERS:
            - process (PROCESS*): process array.
            - n (int): number of processes.

        OUTPUT PARAMETERS:
            - Constructs GANTT chart linked list.

        RETURN VALUES:
            - (GANTT*): pointer to GANTT chart head.

        APPROACH:
            - Sort processes on arrival time, or on burst time in descending order if arrival times are same.
            - At each time unit, run process with longest remaining time.
            - Preempt if necessary.
            - Update GANTT chart and process stats.
    */

    if(n < 1) {
        printf("\nInvalid number of Processes!\n");
        return NULL;
    }

    if(process) {
        for(int i = 0; i < n-1; i++)                // sort according to arrival time
        {
            for(int j = i+1; j < n; j++)
            {
                if(process[i].arrive_time > process[j].arrive_time)
                {
                    PROCESS t = process[i];
                    process[i] = process[j];
                    process[j] = t;
                    continue;
                }
                if(process[i].arrive_time == process[j].arrive_time) {          // if same arrival time, sort on burst time in descending order
                    if(process[i].burst_time < process[j].burst_time) {
                        PROCESS t = process[i];
                        process[i] = process[j];
                        process[j] = t;
                    }
                }
            }
        }
    }
    else {
        printf("\nFailed to read the processes!\n");
        return NULL;
    }

    GANTT *ghead = NULL, *gnew = NULL, *gcurr = NULL;
    int completed = 0, curr_time = 0;

    while (completed < n) {
        int max_idx = -1;
        int max_burst = INT_MIN;

        for (int i = 0; i < n; i++) {
            if (process[i].arrive_time <= curr_time && process[i].burst_time > 0) {
                if (process[i].burst_time > max_burst) {
                    max_burst = process[i].burst_time;
                    max_idx = i;
                }
            }
        }

        GANTT *gnew = (GANTT*)malloc(sizeof(GANTT));

        if(max_idx == -1) {            // system is IDLE
            gnew->pid = -1;
            gnew->start_time = curr_time;
            gnew->finish_time = curr_time + 1;
            gnew->next = NULL;
        }
        else {
            gnew->pid = process[max_idx].pid;
            gnew->start_time = curr_time;
            gnew->finish_time = curr_time + 1;
            gnew->next = NULL;

            process[max_idx].burst_time--;

            if (process[max_idx].burst_time == 0) {
                completed++;
                process[max_idx].turnaround_time = gnew->finish_time - process[max_idx].arrive_time;
                process[max_idx].wait_time = process[max_idx].turnaround_time - process[max_idx].initial_burst;
            }
        }

        if(gcurr) {
            if(max_idx == -1 && gcurr->pid == -1) {
                gcurr->finish_time = gnew->finish_time;
                free(gnew);
                gnew = NULL;
            }
            if(max_idx != -1 && (gcurr->pid == process[max_idx].pid)) {
                gcurr->finish_time = gnew->finish_time;
                free(gnew);
                gnew = NULL;
            }
        }

        if(!ghead) {
            ghead = gcurr = gnew;
        }
        else {
            if(gnew) {
                gcurr->next = gnew;
                gcurr = gnew;
            }
        }
        curr_time++;
    }

    return ghead;
}

void displayGantt(GANTT *ghead) {
    /*
        INTRODUCTION:
            Displays GANTT chart scheduling output.

        INPUT PARAMETERS:
            - ghead (GANTT*): pointer to GANTT chart head.

        OUTPUT PARAMETERS:
            - Prints GANTT chart to console.

        RETURN VALUES:
            - None.

        APPROACH:
            - Traverse linked list and print PID, start and finish times.
    */

    printf("\nGant Chart:-");
    printf("\nStart\tPID\tFinish\n------------------------");
    GANTT *gnew = ghead;
    while(gnew != NULL)
    {
        if(gnew->pid == -1)
            printf("\n%d\tIDLE\t%d\t", gnew->start_time, gnew->finish_time);
        else
            printf("\n%d\tP%d\t%d", gnew->start_time, gnew->pid, gnew->finish_time);

        gnew = gnew->next;
    }
    printf("\n----------------------------------------------------------------\n");
}

void destroyGantt(GANTT *ghead) {
    /*
        INTRODUCTION:
            Frees allocated memory for GANTT chart.

        INPUT PARAMETERS:
            - ghead (GANTT*): pointer to GANTT chart head.

        OUTPUT PARAMETERS:
            - Frees dynamically allocated memory.

        RETURN VALUES:
            - None.

        APPROACH:
            - Traverse linked list and free each node using free().
    */

    GANTT *gcurr = ghead;
    while(ghead) {
        gcurr = ghead;
        ghead = ghead->next;
        free(gcurr);
    }
}

#endif          // SCHEDULER_H