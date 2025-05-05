# HY-486 Distributed Systems – Phase 1
### Food Order Management with Threads

## Overview

This project simulates a multithreaded system for managing food orders. It involves three types of threads:

- **District threads**: Generate food orders and later verify if they have been completed.
- **Agent threads**: Fetch orders from a pending stack and move them into the preparation queue.
- **Cook threads**: Dequeue orders from the preparation queue and mark them as completed by inserting them into the appropriate district’s list based on the order ID.

Each type of thread operates on synchronized shared data structures using mutexes:
- `PendingOrders`: a LIFO stack containing newly created orders.
- `UnderPreparationOrders`: a FIFO queue containing orders being prepared.
- `CompletedOrdersList`: one per district, storing completed order IDs.
- `Checksum`: verifies that all expected orders have been completed correctly.

## Compilation and Execution

The number of threads is set at compile time using the `N_THREADS` flag, which must be divisible by 4.  
By default, `N_THREADS=4` is defined in the Makefile.

### Compile the project:
```bash
make            # Builds with the default N_THREADS=4
make N_THREADS=8  # Builds with a custom thread count
```
### Run the program:  
```bash 
./main 
```
### Clean the Project:
```bash 
make clean 
```

Defaults to `N_THREADS=4`. It **must be divisible by 4**:
- `N_THREADS / 2` are district threads
- `N_THREADS / 4` are agent threads
- `N_THREADS / 4` are cook threads


## Project Structure
```
├── main.c       # Entry point, thread creation, and final verification
├── core.c       # Implementation of all shared data structures and thread routines
├── core.h       # Header file with shared structure definitions and function prototypes
├── Makefile     # Compilation logic with N_THREADS configuration
└── README.md    # Project documentation
```


**Author:** Alexandros Papafragkakis  
**Course:** HY-486 – Distributed Systems  
**Semester:** Spring 2024–2025  
