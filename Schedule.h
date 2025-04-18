// Header file to implement CPU scheduling algorithms

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct process
{
    int pid, arrive_time, burst_time;
    int turnaround_time, wait_time;
    int priority;
}PROCESS;

typedef struct gantt
{
    int pid, start_time, finish_time;
    struct gantt *next;
}GANTT;

PROCESS* getProcesses(int);
int getProcessesFile(char *, PROCESS **);
void displayProcesses(PROCESS *, int);
void destroyProcesses(PROCESS *);

GANTT* getGanttFCFS(PROCESS *, int);
GANTT* getGanttSJF(PROCESS *, int);

void displayGantt(GANTT *);
void destroyGantt(GANTT *);

PROCESS* getProcesses(int n) {
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
    }
    return process;
}

int getProcessesFile(char *fineName, PROCESS **process) {
    FILE *fptr = fopen(fineName, "r");
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
        }
    }
    fclose(fptr);
    return n;
}

void displayProcesses(PROCESS *process, int n) {
    float waitSum = 0, tatSum = 0;
    printf("\n\nWaiting time and Turn-around time:-");
    printf("\nPID\tWait\tTAT\n------------------------");
    for(int i = 0; i < n; i++)
    {
        printf("\nP%d\t%d\t%d", process[i].pid, process[i].wait_time, process[i].turnaround_time);
        waitSum += process[i].wait_time;
        tatSum += process[i].turnaround_time;
    }
    printf("\n\nAverage Waiting time: %.2f", waitSum/n);
    printf("\nAverage Turn-around time: %.2f", tatSum/n);
    printf("\n----------------------------------------------------------------\n");
}

void destroyProcesses(PROCESS *process) {
    if(process) free(process);
}

GANTT* getGanttFCFS(PROCESS *process, int n) {
    if(n < 1) {
        printf("\nInvalid number of Processes!\n");
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
    if(n < 1) {
        printf("\nInvalid number of Processes!\n");
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
        int curr_time = 0, mini = -1;

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
                curr_time = gcurr->finish_time;
                mini = i;

                while(j < n && process[j].arrive_time < curr_time) j++;

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

void displayGantt(GANTT *ghead) {
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
    GANTT *gcurr = ghead;
    while(ghead) {
        gcurr = ghead;
        ghead = ghead->next;
        free(gcurr);
    }
}