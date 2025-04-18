// Program to implement FCFS (First Come First Serve) scheduling algorithm

#include "Schedule.h"

void main() {

    /*
        *** To take all inputs manually ***
        int n = -1;
        do {
            printf("\nEnter number of processes: ");
            scanf("%d", &n);
            if(n < 1) printf("\nInvalid number of processes!\n");
        } while(n < 1);

        PROCESS *processes = getProcesses(n);
    */

    /*
            **** To take file name as input from the user ***
        char fileName[50];
        printf("\nEnter file name: ");
        scanf("%s", fileName);

    */

    PROCESS *processes = NULL;
    int n = getProcessesFile("input.txt", &processes);

    if(processes) {
        for(int i = 0; i < n-1; i++)                // sort according to arrival time
        {
            for(int j = i+1; j < n; j++)
            {
                if(processes[i].arrive_time > processes[j].arrive_time)
                {
                    PROCESS t = processes[i];
                    processes[i] = processes[j];
                    processes[j] = t;
                }
            }
        }

        GANTT *gantt = getGanttFCFS(processes, n);

        if(gantt) displayGantt(gantt);
        displayProcesses(processes, n);
        
        destroyGantt(gantt);
        destroyProcesses(processes);
    }
    else printf("\nFailed to read the processes!\n");
}