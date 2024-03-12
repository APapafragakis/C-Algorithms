/*
Alexandros Papafragakis
CSD5084
*/
#include <stdio.h>       // Include standard input/output library
#include <stdlib.h>      // Include standard library
#include <pthread.h>     // Include pthread (POSIX threads) library
#include <semaphore.h>   // Include semaphores library
#include <unistd.h>      // Include library for sleep function
#include <time.h>        // Include library for time-related functions

#define MAX_STUDENTS 40   // Define the maximum number of students
#define MAX_CAPACITY 8    // Define the maximum capacity of the room

sem_t room_sema, door_sema;         // Declare semaphores for the room and door
pthread_mutex_t waiting_mutex;     // Declare a mutex for managing waiting students
int students_in_room = 0;          // Initialize the number of students in the room
int students_waiting = 0;          // Initialize the number of students waiting
int inside[MAX_CAPACITY];
int outside[MAX_STUDENTS];
int first_waiting = 0;

void student_enter(int student_id) {
    printf("Student %d is entering the room.\n\n", student_id);  // Print a message when a student enters the room
}

void student_exit(int id, int sleeping_time) {
    printf("Student %d exited the study hall after studying for %d secs\n\n",id,sleeping_time+5);
}

void *student(void *student_id) {    // Define the student thread function
    int id = *(int *)student_id;    // Cast the student ID from the argument       

    printf("Student %d was born\n\n",id);
    pthread_mutex_lock(&waiting_mutex);  // Acquire the waiting mutex
    outside[students_waiting] = id;
    students_waiting++;              // Increment the number of waiting students
    printf("Student %d is waiting outside.\n\n", id);
    printf("Study room:   ");
    for (int i = 0; i < MAX_CAPACITY; i++) {
        printf("| ");
        if(inside[i]==0){
            printf("   ");
        }else{
            printf("%2d ", inside[i]);
        }
    }
    printf("|\nWaiting room: ");
    for (int i = first_waiting; i < students_waiting+first_waiting; i++) {
        printf("| ");
        if(outside[i]==0){
            printf("   ");
        }else{
            printf("%2d ", outside[i]);
        }
    }
    printf("|\n\n");
    pthread_mutex_unlock(&waiting_mutex);  // Release the waiting mutex



    sem_wait(&room_sema);      
    while (id!=outside[first_waiting]){
       sem_post(&room_sema);
       sem_wait(&room_sema);
    }
          
    pthread_mutex_lock(&waiting_mutex);  // Acquire the waiting mutex
    
    int pos=0;
    for (int i= 0; i < MAX_CAPACITY; i++)
    {
       if (inside[i]==0){
        pos = i;
        break;
       }
    }
    inside[pos] = id;
    first_waiting++;
    students_in_room++;
    student_enter(id);
    
     printf("Study room:   ");
    for (int i = 0; i < MAX_CAPACITY; i++) {
        printf("| ");
        if(inside[i]==0){
            printf("   ");
        }else{
            printf("%2d ", inside[i]);
        }
    }
    printf("|\nWaiting room: ");
    for (int i = first_waiting; i < students_waiting+first_waiting; i++) {
        printf("| ");
        if(outside[i]==0){
            printf("   ");
        }else{
            printf("%2d ", outside[i]);
        }
    }
    printf("|\n\n");

    pthread_mutex_unlock(&waiting_mutex);  // Release the waiting mutex

    int sleeping_time = rand()%11;
    sleep(sleeping_time+5);

    pthread_mutex_lock(&waiting_mutex);  // Acquire the waiting mutex
    students_in_room--;              // Decrement the number of students in the room
    student_exit(id,sleeping_time);               // Call the student_exit function
    inside[pos] = 0;

     printf("Study room:   ");
    for (int i = 0; i < MAX_CAPACITY; i++) {
        printf("| ");
        if(inside[i]==0){
            printf("   ");
        }else{
            printf("%2d ", inside[i]);
        }
    }
    printf("|\nWaiting room: ");
    for (int i = first_waiting; i < students_waiting+first_waiting; i++) {
        printf("| ");
        if(outside[i]==0){
            printf("   ");
        }else{
            printf("%2d ", outside[i]);
        }
    }
    printf("|\n\n");

    if (students_in_room == 0 && students_waiting > 0) {  // Check if no students are in the room and students are waiting
        int num_entering = students_waiting > MAX_CAPACITY ? MAX_CAPACITY : students_waiting;
        for (int i = 0; i < num_entering; i++) {
            sem_post(&room_sema);    // Allow students to enter the room up to the room's capacity
        }
    }
    pthread_mutex_unlock(&waiting_mutex);  // Release the waiting mutex

    pthread_exit(NULL);              // Terminate the student thread
}

int main() {                        // Define the main function
    srand(0);              // Seed the random number generator with the current time

    int total_students;             // Declare a variable to store the total number of students
    printf("Please enter the number of students N: ");
    scanf("%d", &total_students);    // Prompt the user to input the number of students

    if (total_students < 20 || total_students > 40 ) {
        printf("There must be at least 20 students to start.\n");
        return 1;                   // Check if there are at least 8 students to start
    }

    sem_init(&room_sema, 0, MAX_CAPACITY);   // Initialize the room semaphore with maximum capacity
    pthread_mutex_init(&waiting_mutex, NULL);  // Initialize the waiting mutex

    pthread_t threads[MAX_STUDENTS];         // Declare an array of student threads
    int student_ids[MAX_STUDENTS];           // Declare an array of student IDs

    for (int i = 0; i < total_students; i++) {
        student_ids[i] = i + 1;              // Assign unique IDs to students
        pthread_create(&threads[i], NULL, student, &student_ids[i]);  // Create student threads
    }

    for (int i = 0; i < total_students; i++) {
        pthread_join(threads[i], NULL);       // Wait for all student threads to complete
    }

    sem_destroy(&room_sema);                  // Destroy the room semaphore
    sem_destroy(&door_sema);                  // Destroy the door semaphore
    pthread_mutex_destroy(&waiting_mutex);    // Destroy the waiting mutex

    return 0;                                 // Exit the program
}
