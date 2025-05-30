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

    PROCESS *process = NULL;
    int n = getProcessesFile("input.txt", &process);

    if(n > 0) {
        GANTT *gantt = getGanttFCFS(process, n);

        if(gantt) displayGantt(gantt);
        displayProcesses(process, n);
        
        destroyGantt(gantt);
        destroyProcesses(process);
    }
}