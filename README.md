# CPU Scheduling Algorithms in C

This repository contains C implementations of various **CPU scheduling algorithms** typically covered in Operating Systems courses.

Each algorithm constructs a Gantt chart to visualize scheduling, and calculates average waiting and turnaround times for given processes.

---

## Features

### Implemented Scheduling Algorithms:
- **FCFS (First Come First Serve)**
- **SJF (Shortest Job First)**
- **HRRN (Highest Response Ratio Next)**
- **LJF (Longest Job First)**
- **Priority Scheduling (Non-Preemptive)**
- **SRTF (Shortest Remaining Time First)**
- **LRTF (Longest Remaining Time First)**
- **Priority Scheduling (Preemptive)**
- **Round Robin**

### User-defined header file**: `Schedule.h` containing:
- Structure definitions for **`PROCESS`** and **`GANTT`** chart nodes
- Functions to:
  - Read input (manually or via file)
  - Create and manage Gantt charts for each scheduling algorithm
  - Display Gantt charts and process stats
  - Calculate average waiting and turnaround times
  - Clean up allocated memory

### Flexible Input Handling:
- Manual process input (through console)
- Input via file (`input.txt` or `input_priority.txt`)

### Supports IDLE times in the Gantt chart if CPU remains idle at any time

---

## Input Format

### For `input.txt`
```
<number of processes>
<arrival time> <burst time>
```

### For `input_priority.txt`
```
<number of processes>
<arrival time> <burst time> <priority>
```

### Example
```
4
0 5 2
1 3 3
2 8 1
3 6 4
```

## Author
