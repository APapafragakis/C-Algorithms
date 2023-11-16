# Student Reading Room Synchronization Program

## Overview
This C program, developed by Alexandros Papafragakis, simulates a student reading room in a school where there is space for 8 students. The program uses threads and semaphores to enforce rules for student entry, ensuring that no more than 8 students are present in the reading room at the same time.

## Rules for Student Entry
- If there are 7 or fewer students in the reading room, a new student can enter and study normally.
- If there are already 8 students in the reading room, a new student must wait outside until the room is completely empty (i.e., all 8 existing students leave) before entering.

## Implementation Details
- Each student is represented by a separate thread.
- The program avoids starvation by allowing students in the corridor to enter the reading room in a first-come-first-served (FIFO) order.
- The program terminates when all students have finished studying.

## Student Initialization
1. The initialization of student threads includes assigning a student ID (AM).
2. Study time for each student is randomly set between 5 to 15 seconds using the `random(3)` library.
3. Students can be either in the reading room or in the corridor at any given time.

## Reading Room Initialization
- The reading room accommodates up to 8 students and starts empty.
- The total number of students wishing to study is between 20 and 40, provided during program initialization.

## Usage Notes
- The `sleep(1)` function is used for ease of implementation.
- Appropriate messages are printed to indicate the status of the reading room and corridor whenever a student thread is created, a student enters the reading room, a student finishes studying and leaves the room, and when a student cannot enter the room and must wait in the corridor.

## Example Execution
- The program begins execution, specifying the number of students. Student threads are created and start filling the reading room.

## Instructions for Compilation and Execution
1. Compile the program using a C compiler (e.g., `gcc -o reading_room reading_room.c -lpthread`).
2. Run the compiled executable (e.g., `./reading_room`).

Enjoy simulating the student reading room experience!

-Assingment 2 for Operating System CS-345 in Computer Science Department.
