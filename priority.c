// Program to implement Priority (non-preemptive) scheduling algorithm.

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
        **** To take file name input from the user ***
        char fileName[50];
        printf("\nEnter file name: ");
        scanf("%s", fileName);
    */

    PROCESS *process = NULL;
    int n = getProcessesFilePriority("input_priority.txt", &process);

    if(n > 0) {
        GANTT *gantt = getGanttPriority(process, n);

        if(gantt) displayGantt(gantt);
        displayProcesses(process, n);
        
        destroyGantt(gantt);
        destroyProcesses(process);
    }
}