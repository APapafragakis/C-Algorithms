#include <stdio.h>      
#include "core.h"

int main() {
    printf("N_THREADS: %d\n\n", N_THREADS);
    fflush(stdout);

    // Initialize data structures
    stack_init(&PendingOrders);
    queue_init(&UnderPreparationOrders);

    for (size_t i = 0; i < DIST; ++i) {
        list_init(&Districts[i].completedOrders);
        Districts[i].checksum = 0;
        pthread_mutex_init(&Districts[i].checksum_lock, NULL);
    }

    // Create district threads
    pthread_t district_threads[DIST];
    struct DistrictArgs district_args[DIST];
    for (size_t i = 0; i < DIST; ++i) {
        district_args[i].tid = i;
        pthread_create(&district_threads[i], NULL, district_thread, &district_args[i]);
    }

    // Create agent threads
    pthread_t agent_threads[AGNT];
    for (size_t i = 0; i < AGNT; ++i) {
        pthread_create(&agent_threads[i], NULL, agent_thread, (void*)i);
    }

    // Create cook threads
    pthread_t cook_threads[COOK];
    for (size_t i = 0; i < COOK; ++i) {
        pthread_create(&cook_threads[i], NULL, cook_thread, (void*)i);
    }

    // Wait for all threads to complete
    for (size_t i = 0; i < DIST; ++i) {
        pthread_join(district_threads[i], NULL);
    }
    for (size_t i = 0; i < AGNT; ++i) {
        pthread_join(agent_threads[i], NULL);
    }
    for (size_t i = 0; i < COOK; ++i) {
        pthread_join(cook_threads[i], NULL);
    }

    // Verify results
    verify_districts();

    // Check if pending orders stack is empty
    bool stack_empty = (atomic_load(&PendingOrders.head) == NULL);
    printf("%s PendingOrders Empty %d\n", PassStr(stack_empty), stack_empty ? 0 : 1);

    // Check if under-preparation queue is empty
    size_t underPrepSize = queue_size(&UnderPreparationOrders);
    bool queue_empty = (underPrepSize == 0);
    printf("%s UnderPreparationOrders Empty %zu\n", PassStr(queue_empty), underPrepSize);

    return 0;
}