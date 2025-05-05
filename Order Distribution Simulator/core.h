#ifndef CORE_H
#define CORE_H

#include <unistd.h>
#include <stddef.h>     
#include <stdbool.h>   
#include <pthread.h>
#include <stdio.h>     
#include <stdlib.h>     
#include <stdatomic.h>  // For atomic operations
 
#define DIST (N_THREADS / 2)
#define AGNT (N_THREADS / 4)
#define COOK (N_THREADS / 4)

// Constants for elimination array
#define MIN_BACKOFF 5
#define MAX_BACKOFF 15
#define ELIMINATION_ARRAY_SIZE 64

typedef size_t Order;

// For lock-free stack
struct StackNode {
    Order value;
    _Atomic(struct StackNode*) next;
};

// Improved elimination array structure
typedef struct {
    _Atomic bool occupied;
    _Atomic Order value;
} EliminationSlot;

struct PendingOrdersStack {
    _Atomic(struct StackNode*) head;
    // Elimination array fields
    EliminationSlot eliminationArray[ELIMINATION_ARRAY_SIZE];
};

// For total queue
struct QueueNode {
    Order value;
    struct QueueNode* next;
};

struct UnderPreparationOrdersQueue {
    struct QueueNode* head;
    struct QueueNode* tail;
    pthread_mutex_t lock;
};

// For lazy synchronized list
struct ListNode {
    Order value;
    struct ListNode* next;
    _Atomic bool marked;  // For lazy deletion
    pthread_mutex_t lock; // Per-node lock
};

struct CompletedOrdersList {
    struct ListNode* head;
};

struct District {
    struct CompletedOrdersList completedOrders;
    size_t checksum;
    pthread_mutex_t checksum_lock;
};

struct DistrictArgs {
    size_t tid;
};

extern struct PendingOrdersStack PendingOrders;
extern struct UnderPreparationOrdersQueue UnderPreparationOrders;
extern struct District Districts[DIST];

// Lock-free stack operations
void stack_init(struct PendingOrdersStack* stack);
void stack_push(struct PendingOrdersStack* stack, Order value);
Order stack_pop(struct PendingOrdersStack* stack, bool* success);
bool tryPush(struct PendingOrdersStack* stack, struct StackNode* node);
Order tryPop(struct PendingOrdersStack* stack, bool* success);
Order visit_elimination_array(struct PendingOrdersStack* stack, Order value, bool is_push);

// Queue operations
void queue_init(struct UnderPreparationOrdersQueue* queue);
void queue_enqueue(struct UnderPreparationOrdersQueue* queue, Order value);
Order queue_dequeue(struct UnderPreparationOrdersQueue* queue, bool* success);
size_t queue_size(struct UnderPreparationOrdersQueue* queue);

// Lazy synchronized list operations
void list_init(struct CompletedOrdersList* list);
bool list_search(struct CompletedOrdersList* list, Order value);
void list_insert(struct CompletedOrdersList* list, Order value);
bool list_delete(struct CompletedOrdersList* list, Order value);
size_t list_size(struct CompletedOrdersList* list);
size_t list_sum(struct CompletedOrdersList* list);
bool validate(struct ListNode* pred, struct ListNode* curr);

// Thread functions
void* district_thread(void* arg);
void* agent_thread(void* arg);
void* cook_thread(void* arg);

// Utility functions
void verify_districts(void);
const char* PassStr(bool pass);
void backoff(int min, int max);

#endif // CORE_H