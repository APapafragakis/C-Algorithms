# Assignment 2: Multi-threaded Bus Scheduling System

**Course:** HY-345 Operating Systems  
**Semester:** Winter 2024  
**Author:** Alexandros Papafragkakis (csd5084)  

---

## Description
This program implements a **multi-threaded bus scheduling simulation** using **POSIX threads** and **semaphores**. The goal is to manage the transportation of students between two stops (Stop A and Stop B), their stay at the university, and their return home under specific constraints:

1. **Bus Capacity:** Maximum of 12 students per trip, with a limit of 3 students from each department.
2. **FIFO Order:** Students are served in the order they arrive.
3. **Avoid Starvation:** No student waits indefinitely.
4. **Random Study Time:** Each student spends between 5 and 15 seconds studying at the university.

---

## Features
- Each student is represented as a thread, with properties like department, study time, and priority.
- A dedicated thread for the bus manages student boarding, travel between stops, and synchronization.
- Semaphores ensure proper synchronization between threads.
- Mutex locks safeguard critical sections to prevent race conditions.

### Real-time Logs
The program provides detailed logs during execution, such as:
- Student arrival at stops.
- Boarding and disembarking from the bus.
- Arrival at the university and return home.
- Waiting due to department quota.

---

## Compilation and Execution

### Compilation
Use the provided `Makefile` for easy compilation:

```makefile
CC = gcc
CFLAGS = -pthread
TARGET = semaphores

SRC = semaphores.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
```
## Compile the program
```bash
make all
```
## Run the program
```bash
./semaphores
```

### Enter the number of students when prompted
### Example:
```bash
 Enter the number of students:
 20
```

## Clean up compiled files
```bash
make clean
