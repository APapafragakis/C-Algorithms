# HY-486 Distributed Systems – Phase 1  
### Multithreaded Food Order Management System

## Overview

This project simulates a **concurrent system** for managing food orders, implemented in **C with pthreads**. It uses multiple threads operating over shared data structures with different access patterns and synchronization strategies.

### Data Structure Design

This project makes use of advanced concurrent data structures tailored for multithreaded environments:

- An **Unbounded Lock-Free Elimination BackOff Stack** for storing pending food orders. It allows high-throughput concurrent push/pop operations by enabling threads to exchange values directly via an elimination array, reducing contention on the shared stack head.

- An **Unbounded Total Queue** for orders under preparation. It provides FIFO ordering and thread-safe access using a single mutex for simplicity and reliability.

- An **Unbounded Linked List with Lazy Synchronization** for each district’s list of completed orders. It uses fine-grained locking and deferred deletion to support safe concurrent insertions and searches.

These structures were chosen to balance performance and correctness across multiple thread types that interact intensively.

### Thread Roles

- **District threads**  
  Generate unique food orders, insert them into the pending stack, and later wait until all their orders are marked as completed to compute their checksum.

- **Agent threads**  
  Remove orders from the pending stack and place them into the preparation queue.

- **Cook threads**  
  Retrieve orders from the preparation queue and insert them into the appropriate completed list of each district based on the order ID.

The system ensures correctness and consistency by using synchronized or lock-free shared data structures.



## Shared Data Structures

- **PendingOrders**  
  A central stack where district threads push newly created orders.  
  ➤ Implemented as an **Unbounded Lock-Free Elimination BackOff Stack**. It uses atomic operations and an elimination array to reduce contention between threads.

- **UnderPreparationOrders**  
  A queue where agent threads enqueue orders for cooks to prepare.  
  ➤ Implemented as an **Unbounded Total Queue** protected by a single `pthread_mutex_t` for thread-safe FIFO access.

- **CompletedOrdersList**  
  Each district has its own list where cook threads insert completed orders.  
  ➤ Implemented as an **Unbounded Linked List with Lazy Synchronization** using per-node locks and a logical deletion flag (`marked`).

- **Checksum**  
  Each district maintains a sum of the completed order IDs to verify correctness.  
  ➤ It is a `size_t` value, protected by a `pthread_mutex_t`.


### Structure Features

- **Elimination Stack**  
  Uses lock-free CAS operations with an `elimination array` to reduce contention between push/pop operations. Enables direct data exchange between producer and consumer threads.

- **Total Queue**  
  FIFO queue protected by a single mutex, used for reliable inter-thread communication (agent → cook).

- **Lazy-Synchronized List**  
  Fine-grained locking per node. Supports safe concurrent insertions and searches using a `validate(pred, curr)` mechanism and logical deletion (via a `marked` flag).

---

## Compilation and Execution

The number of threads is set at compile time using the `N_THREADS` flag, which must be divisible by 4:

```bash
make run          # Builds and runs with N_THREADS=4 by default
make N_THREADS=8  # Custom thread count


## Project Structure

- `main.c`: Starts all threads and verifies results.
- `core.c` / `core.h`: Contain all shared data structures and thread logic.
- `Makefile`: Builds the project and allows thread count customization.

**Author:** Alexandros Papafragkakis  
**Course:** HY-486 – Distributed Systems  
**Semester:** Spring 2024–2025  