# Core Process Scheduling and Resource Management Simulation

A simulation program for process scheduling and synchronization in a time-shared system with multiple processing cores, a solid-state drive, and 64 locks. This project aims to provide a practical understanding of process execution and resource management in a multi-core environment.

<br>

### ☆ ──── Technologies ──── ☆

• C++ <br>
• Input redirection <br>
<br>

### ☆ ──── Features ──── ☆

• **Core Scheduling:** Implements a First Come-First Served policy for scheduling core usage, managing processes in a FIFO ready queue.
<br>
• **SSD Scheduling:** Handles SSD accesses in mutual exclusion following a First Come-First Served policy, similar to core scheduling. <br>
• **Lock Management:** Manages a set of 64 locks, allowing processes to access critical shared resources in mutual exclusion. <br>
• **Detailed Output:** Provides detailed summary reports at each process start or termination, including current simulated time, process sequence number, busy core count, ready queue contents, and process states (READY, RUNNING, BLOCKED, TERMINATED) for processes in main memory. <br> <br>
<br>
