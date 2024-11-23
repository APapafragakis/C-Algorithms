/* ALEXANDROS PAPAFRAGKAKIS
CSD5084
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define STUDYTIME_MIN 5          
#define STUDYTIME_MAX 15    
#define BUS_CAPACITY 12        
#define MAX_STUDENTS_PER_DEPT (BUS_CAPACITY / 4)  


sem_t bus_sem, stopA_sem, stopB_sem, home_sem;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexB = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexBus = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexPrio = PTHREAD_MUTEX_INITIALIZER;

int students_returning = 0;
int num_students;

//Counters for students 
int students_at_stopA = 0;
int priority_stop_A = 0;
int students_at_stopB = 0;
int dept_counts[4] = {0, 0, 0, 0};

//Student details
typedef struct {
    int id;
    int department;
    int study_time;
    int priority;
} Student;

Student* student_arr;


void print_students(){
    printf("Students: \n");
    for(int i = 0; i < num_students; i++){
        printf("\tStudent %d from department %d with priority %d\n", student_arr[i].id, student_arr[i].department, student_arr[i].priority);
    }
}

//Student thread
void *student_thread(void *arg) {
    Student *student = (Student *)arg;

    //Stop A
    pthread_mutex_lock(&mutex);
    students_at_stopA++;
    student->priority = students_at_stopA;
    printf("Student %d from department %d arrived at Stop A.\n", student->id, student->department);
    pthread_mutex_unlock(&mutex);

stop_A_label:

    sem_wait(&stopA_sem);

    pthread_mutex_lock(&mutexPrio);
    if(student->priority != priority_stop_A) {
        pthread_mutex_unlock(&mutexPrio);
        sem_post(&stopA_sem);
        goto stop_A_label; // sorry 
    }
    pthread_mutex_unlock(&mutexPrio);


    //Board the bus
    pthread_mutex_lock(&mutexBus);

    if (dept_counts[student->department] > 0) {
        dept_counts[student->department]--;
        students_at_stopA--;
        
        pthread_mutex_lock(&mutexPrio);
        for(int i = 0; i < num_students; i++){
            if(student_arr[i].priority > student->priority) student_arr[i].priority--;
        }
        student->priority = 0;
        pthread_mutex_unlock(&mutexPrio);


        printf("Student %d from department %d boarded the bus.\n", student->id, student->department);
        pthread_mutex_unlock(&mutexBus);
        
        
        sem_wait(&bus_sem);

        // Study at the university
        printf("Student %d went to University\n", student->id);

        sleep(student->study_time);

        // After studying - head to Stop B
        pthread_mutex_lock(&mutexB);
        printf("Student %d from department %d arrived at Stop B after studying for %d seconds.\n", student->id, student->department, student->study_time);
        students_at_stopB++;
        pthread_mutex_unlock(&mutexB);
        // sem_post(&stopB_sem);

        // Wait for the bus 
        sem_wait(&stopB_sem);
        pthread_mutex_lock(&mutexBus);
        students_at_stopB--;
        students_returning++;
        printf("Student %d from department %d boarded the bus.\n", student->id, student->department);
        pthread_mutex_unlock(&mutexBus);

        sem_wait(&home_sem);
    
        pthread_mutex_lock(&mutexBus);
        printf("Student %d returned home\n", student->id);
        pthread_mutex_unlock(&mutexBus);
    } else {
        priority_stop_A++;
        printf("Student %d from department %d waits at Stop A due to department quota.\n", student->id, student->department);
        pthread_mutex_unlock(&mutexBus);

        goto stop_A_label;
    }

    return NULL;
}

//Bus thread
void *bus_thread(void *arg) {

    pthread_mutex_lock(&mutexA);
    printf("\nBus arrived at stop A.\n\n");
    pthread_mutex_unlock(&mutexA);

    while (1) {
        for(int i = 0; i < 4; i++) {
            dept_counts[i] = MAX_STUDENTS_PER_DEPT;
        }

        sleep(2);

        priority_stop_A = 1;
        for (int i = 0; i < students_at_stopA; i++) {
            sem_post(&stopA_sem);
        }

        sleep(2);

        int students_boarded = BUS_CAPACITY;
        for(int i = 0; i < 4; i++) {
            students_boarded -= dept_counts[i];
        }

        int temp_max = (BUS_CAPACITY < students_at_stopA) ? BUS_CAPACITY : students_at_stopA;
        for(int i = 0; i < temp_max - students_boarded; i++) {
            sem_wait(&bus_sem);
        }

        pthread_mutex_lock(&mutexA);
        printf("\nBus departing from Stop A\n\n");
        pthread_mutex_unlock(&mutexA);

        sleep(10);

        pthread_mutex_lock(&mutexA);
        printf("\nBus arrived at Stop B.\n\n");
        pthread_mutex_unlock(&mutexA);

        for(int i = 0; i < students_boarded; i++) {
            sem_post(&bus_sem);
        }

        sleep(2);
        printf("\n");

        for (int i = 0; i < students_at_stopB; i++) {
            sem_post(&stopB_sem);
        }

        sleep(2); 

        pthread_mutex_lock(&mutexB);
        printf("\nBus departing from Stop B.\n\n");
        pthread_mutex_unlock(&mutexB);

        sleep(10);

        pthread_mutex_lock(&mutexA);
        printf("\nBus arrived at stop A.\n\n");
        pthread_mutex_unlock(&mutexA);

        for(int i = 0; i < students_returning; i++) {
            sem_post(&home_sem);
        }

        printf("\n");

        students_returning = 0;
    }

    return NULL;
}

int main() {
    printf("Enter the number of students: \n");
    scanf("%d", &num_students);

    pthread_t bus;
    pthread_t student_threads[num_students];

    student_arr = (Student *)malloc(num_students * sizeof(Student));


    sem_init(&bus_sem, 0, 0);
    sem_init(&stopA_sem, 0, 0);
    sem_init(&stopB_sem, 0, 0);
    sem_init(&home_sem, 0, 0);

    srand(time(NULL));

    for (int i = 0; i < num_students; i++) {
        student_arr[i].id = i + 1;
        student_arr[i].department = rand() % 4; 
        student_arr[i].study_time = STUDYTIME_MIN + rand() % (STUDYTIME_MAX - STUDYTIME_MIN + 1);

        pthread_create(&student_threads[i], NULL, student_thread, &student_arr[i]);
    }

    sleep(1);

    pthread_create(&bus, NULL, bus_thread, NULL);


    for (int i = 0; i < num_students; i++) {
        pthread_join(student_threads[i], NULL);
    }

    pthread_cancel(bus);

    // Clean up 
    sem_destroy(&bus_sem);
    sem_destroy(&stopA_sem);
    sem_destroy(&stopB_sem);
    pthread_mutex_destroy(&mutex);

    printf("\nAll students have returned home.\n");
    return 0;
}
