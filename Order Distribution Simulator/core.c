#include "core.h"
#include <time.h>

struct PendingOrdersStack PendingOrders;
struct UnderPreparationOrdersQueue UnderPreparationOrders;
struct District Districts[DIST];

// Helper function for random backoff
void backoff(int min, int max) {
    int delay = min + rand() % (max - min + 1);
    usleep(delay * 1000);
}

// Initialize stack
void stack_init(struct PendingOrdersStack* stack) {
    atomic_store(&stack->head, NULL);
    
    // Initialize elimination array
    for (int i = 0; i < ELIMINATION_ARRAY_SIZE; i++) {
        atomic_store(&stack->eliminationArray[i].occupied, false);
        atomic_store(&stack->eliminationArray[i].value, 0);
    }
    
    // Seed random number generator for elimination array
    srand(time(NULL));
}

// Try to push item to stack
bool tryPush(struct PendingOrdersStack* stack, struct StackNode* node) {
    struct StackNode* oldTop = atomic_load(&stack->head);
    atomic_store(&node->next, oldTop);
    return atomic_compare_exchange_strong(&stack->head, &oldTop, node);
}

// Push to stack
void stack_push(struct PendingOrdersStack* stack, Order value) {
    struct StackNode* node = malloc(sizeof(struct StackNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed in stack_push\n");
        return;
    }
    
    node->value = value;
    atomic_store(&node->next, NULL);
    
    // Try direct push first
    if (tryPush(stack, node)) {
        return;
    }
    
    // If direct push fails, try elimination array
    Order result = visit_elimination_array(stack, value, true);
    if (result != 0) { // Elimination successful
        // free(node);
        return;
    }
    
    // If elimination fails, retry with backoff
    while (true) {
        if (tryPush(stack, node)) {
            return;
        }
        backoff(MIN_BACKOFF, MAX_BACKOFF);
    }
}

// Try to pop from stack
Order tryPop(struct PendingOrdersStack* stack, bool* success) {
    struct StackNode* oldTop = atomic_load(&stack->head);
    if (oldTop == NULL) {
        *success = false;
        return 0;
    }
    
    struct StackNode* newTop = atomic_load(&oldTop->next);
    if (atomic_compare_exchange_strong(&stack->head, &oldTop, newTop)) {
        Order val = oldTop->value;
        free(oldTop);
        *success = true;
        return val;
    }
    
    *success = false;
    return 0;
}

// Pop from stack
Order stack_pop(struct PendingOrdersStack* stack, bool* success) {
    // Try direct pop first
    Order result = tryPop(stack, success);
    if (*success) {
        return result;
    }
    
    // If direct pop fails, try elimination array
    result = visit_elimination_array(stack, 0, false);
    if (result != 0) { // Elimination successful
        *success = true;
        return result;
    }
    
    // If elimination fails, retry with backoff
    while (true) {
        result = tryPop(stack, success);
        if (*success) {
            return result;
        }
        
        // Try elimination again
        result = visit_elimination_array(stack, 0, false);
        if (result != 0) {
            *success = true;
            return result;
        }
        
        backoff(MIN_BACKOFF, MAX_BACKOFF);
    }
}

// Improved elimination array implementation
Order visit_elimination_array(struct PendingOrdersStack* stack, Order value, bool is_push) {
    int slot = rand() % ELIMINATION_ARRAY_SIZE;
    
    if (is_push) {
        // For push: try to place the value in an empty slot
        bool expected = false;
        if (atomic_compare_exchange_strong(&stack->eliminationArray[slot].occupied, &expected, true)) {
            atomic_store(&stack->eliminationArray[slot].value, value);
            
            // Wait a bit for a pop operation to take the value
            for (int i = 0; i < 1000; i++) {
                if (!atomic_load(&stack->eliminationArray[slot].occupied)) {
                    // Value was taken by a pop operation
                    return value; // Signal success
                }
                usleep(1);
            }
            
            // Timeout: try to reclaim the slot
            expected = true;
            if (atomic_compare_exchange_strong(&stack->eliminationArray[slot].occupied, &expected, false)) {
                return 0; // No exchange happened
            } else {
                // Someone took our value
                return value; // Signal success
            }
        }
    } else {
        // For pop: try to find a non-empty slot
        bool occupied = atomic_load(&stack->eliminationArray[slot].occupied);
        if (occupied) {
            bool expected = true;
            if (atomic_compare_exchange_strong(&stack->eliminationArray[slot].occupied, &expected, false)) {
                return atomic_load(&stack->eliminationArray[slot].value); // Got a value
            }
        }
    }
    
    return 0; // No exchange happened
}

// Initialize queue
void queue_init(struct UnderPreparationOrdersQueue* queue) {
    queue->head = NULL;
    queue->tail = NULL;
    pthread_mutex_init(&queue->lock, NULL);
}

// Enqueue item
void queue_enqueue(struct UnderPreparationOrdersQueue* queue, Order value) {
    struct QueueNode* node = malloc(sizeof(struct QueueNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed in queue_enqueue\n");
        return;
    }
    
    node->value = value;
    node->next = NULL;

    pthread_mutex_lock(&queue->lock);
    if (queue->tail == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    pthread_mutex_unlock(&queue->lock);
}

// Dequeue item
Order queue_dequeue(struct UnderPreparationOrdersQueue* queue, bool* success) {
    pthread_mutex_lock(&queue->lock);
    if (queue->head == NULL) {
        *success = false;
        pthread_mutex_unlock(&queue->lock);
        return 0;
    }
    
    struct QueueNode* node = queue->head;
    queue->head = node->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    pthread_mutex_unlock(&queue->lock);
    
    Order val = node->value;
    free(node);
    *success = true;
    return val;
}

// Get queue size
size_t queue_size(struct UnderPreparationOrdersQueue* queue) {
    pthread_mutex_lock(&queue->lock);
    size_t count = 0;
    struct QueueNode* curr = queue->head;
    while (curr) {
        count++;
        curr = curr->next;
    }
    pthread_mutex_unlock(&queue->lock);
    return count;
}

// Initialize lazy list
void list_init(struct CompletedOrdersList* list) {
    struct ListNode* sentinel = malloc(sizeof(struct ListNode));
    if (sentinel == NULL) {
        fprintf(stderr, "Memory allocation failed in list_init\n");
        return;
    }
    
    sentinel->value = 0;
    sentinel->next = NULL;
    atomic_store(&sentinel->marked, false);
    pthread_mutex_init(&sentinel->lock, NULL);
    list->head = sentinel;
}

// Validate list nodes for lazy synchronization
bool validate(struct ListNode* pred, struct ListNode* curr) {
    // Added NULL check for curr
    return pred != NULL && 
           !atomic_load(&pred->marked) && 
           (curr == NULL || !atomic_load(&curr->marked)) && 
           pred->next == curr;
}

// Search for item in lazy list
bool list_search(struct CompletedOrdersList* list, Order value) {
    if (list == NULL || list->head == NULL) {
        return false;
    }
    
    struct ListNode* curr = list->head;
    while (curr && curr->value < value) {
        curr = curr->next;
    }
    return curr != NULL && curr->value == value && !atomic_load(&curr->marked);
}

// Insert item into lazy list
void list_insert(struct CompletedOrdersList* list, Order value) {
    if (list == NULL || list->head == NULL) {
        return;
    }
    
    struct ListNode* pred;
    struct ListNode* curr;
    bool success = false;
    
    while (!success) {
        pred = list->head;
        curr = pred->next;
        
        // Find position
        while (curr && curr->value < value) {
            pred = curr;
            curr = curr->next;
        }
        
        // Lock nodes
        pthread_mutex_lock(&pred->lock);
        if (curr) pthread_mutex_lock(&curr->lock);
        
        // Validate
        if (validate(pred, curr)) {
            if (curr && curr->value == value) {
                // Value already exists
                success = true;
            } else {
                // Insert new node
                struct ListNode* node = malloc(sizeof(struct ListNode));
                if (node == NULL) {
                    fprintf(stderr, "Memory allocation failed in list_insert\n");
                    success = true; // Set success to exit loop
                } else {
                    node->value = value;
                    atomic_store(&node->marked, false);
                    pthread_mutex_init(&node->lock, NULL);
                    node->next = curr;
                    pred->next = node;
                    success = true;
                }
            }
        }
        
        // Unlock
        pthread_mutex_unlock(&pred->lock);
        if (curr) pthread_mutex_unlock(&curr->lock);
    }
}

// Delete item from lazy list
bool list_delete(struct CompletedOrdersList* list, Order value) {
    if (list == NULL || list->head == NULL) {
        return false;
    }
    
    struct ListNode* pred;
    struct ListNode* curr;
    bool result = false;
    bool success = false;
    
    while (!success) {
        pred = list->head;
        curr = pred->next;
        
        // Find position
        while (curr && curr->value < value) {
            pred = curr;
            curr = curr->next;
        }
        
        // Lock nodes
        pthread_mutex_lock(&pred->lock);
        if (curr) pthread_mutex_lock(&curr->lock);
        
        // Validate
        if (validate(pred, curr)) {
            if (curr && curr->value == value) {
                // Mark for deletion (logical deletion)
                atomic_store(&curr->marked, true);
                // Physical removal
                pred->next = curr->next;
                result = true;
            }
            success = true;
        }
        
        // Unlock
        pthread_mutex_unlock(&pred->lock);
        if (curr) pthread_mutex_unlock(&curr->lock);
    }
    
    return result;
}

// Get list size
size_t list_size(struct CompletedOrdersList* list) {
    if (list == NULL || list->head == NULL) {
        return 0;
    }
    
    size_t count = 0;
    struct ListNode* curr = list->head->next; // Skip sentinel
    
    while (curr) {
        if (!atomic_load(&curr->marked)) { // Count only unmarked nodes
            count++;
        }
        curr = curr->next;
    }
    
    return count;
}

// Get sum of list items
size_t list_sum(struct CompletedOrdersList* list) {
    if (list == NULL || list->head == NULL) {
        return 0;
    }
    
    size_t sum = 0;
    struct ListNode* curr = list->head->next; // Skip sentinel
    
    while (curr) {
        if (!atomic_load(&curr->marked)) { // Sum only unmarked nodes
            sum += curr->value;
        }
        curr = curr->next;
    }
    
    return sum;
}

// District thread function
void* district_thread(void* arg) {
    struct DistrictArgs* args = (struct DistrictArgs*)arg;
    size_t tid = args->tid;
    
    fflush(stdout);

    // Push orders to pending orders stack
    for (size_t idx = 0; idx < DIST; ++idx) {
        size_t order_id = tid * DIST + idx;
        stack_push(&PendingOrders, order_id);
        fflush(stdout);
    }

    size_t found = 0;
    bool* found_flags = calloc(DIST, sizeof(bool));
    if (found_flags == NULL) {
        fprintf(stderr, "Memory allocation failed in district_thread\n");
        return NULL;
    }

    // Wait for orders to be completed
    while (found < DIST) {
        for (size_t idx = 0; idx < DIST; ++idx) {
            if (found_flags[idx]) continue;
            size_t order_id = tid * DIST + idx;

            if (list_search(&Districts[tid].completedOrders, order_id)) {
                pthread_mutex_lock(&Districts[tid].checksum_lock);
                Districts[tid].checksum += order_id;
                pthread_mutex_unlock(&Districts[tid].checksum_lock);
                found_flags[idx] = true;
                found++;
                fflush(stdout);
            }
        }
        usleep(1000);
    }

    free(found_flags);
    fflush(stdout);
    return NULL;
}

// Agent thread function
void* agent_thread(void* arg) {
    (void)arg;
    fflush(stdout);
    
    int timeout_counter = 0;
    const int MAX_TIMEOUT = 1000000; // Adjust as needed
    
    for (size_t i = 0; i < 2 * DIST; ++i) {
        bool success = false;
        Order order;
        timeout_counter = 0;
        
        // Pop from pending orders stack
        while (!success && timeout_counter < MAX_TIMEOUT) {
            order = stack_pop(&PendingOrders, &success);
            if (!success) {
                usleep(100);
                timeout_counter++;
            }
        }
        
        if (timeout_counter >= MAX_TIMEOUT) {
            fflush(stdout);
            break;
        }
        
        // Enqueue to under-preparation queue
        queue_enqueue(&UnderPreparationOrders, order);
        fflush(stdout);
    }
    
    fflush(stdout);
    return NULL;
}

// Cook thread function
void* cook_thread(void* arg) {
    (void)arg;
    fflush(stdout);
    
    int timeout_counter = 0;
    const int MAX_TIMEOUT = 1000000;
    
    for (size_t i = 0; i < 2 * DIST; ++i) {
        timeout_counter = 0;
        bool success = false;
        Order order_id = 0;
        
        // Dequeue from under-preparation queue
        while (!success && timeout_counter < MAX_TIMEOUT) {
            order_id = queue_dequeue(&UnderPreparationOrders, &success);
            if (!success) {
                usleep(100);
                timeout_counter++;
            }
        }
        
        if (timeout_counter >= MAX_TIMEOUT) {
            fflush(stdout);
            break;
        }

        // Insert into completed orders list
        size_t tid = order_id / DIST;
        // Make sure tid is valid
        if (tid < DIST) {
            list_insert(&Districts[tid].completedOrders, order_id);
        } else {
            fprintf(stderr, "Invalid tid: %zu\n", tid);
        }
        fflush(stdout);
    }
    
    fflush(stdout);
    return NULL;
}

// Utility function for test results
const char* PassStr(bool pass) {
    return pass ? "PASS" : "FAIL";
}

// Verify districts
void verify_districts(void) {
    for (size_t tid = 0; tid < DIST; ++tid) {
        size_t size = list_size(&Districts[tid].completedOrders);
        size_t sum = list_sum(&Districts[tid].completedOrders);
        size_t checksum;

        pthread_mutex_lock(&Districts[tid].checksum_lock);
        checksum = Districts[tid].checksum;
        pthread_mutex_unlock(&Districts[tid].checksum_lock);

        bool valid_size = (size == DIST);
        bool valid_sum = (sum == (tid * DIST * DIST + ((DIST - 1) * DIST) / 2));
        bool valid_checksum = (checksum == sum);

        printf("%s District[%zu].completedOrders Size %zu\n", PassStr(valid_size), tid, size);
        printf("%s District[%zu].completedOrders Sum %zu\n", PassStr(valid_sum), tid, sum);
        printf("%s District[%zu].completedOrders Valid %zu\n", PassStr(valid_checksum), tid, checksum);
    }
}