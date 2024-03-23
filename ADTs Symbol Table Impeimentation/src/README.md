# SymTable Abstract Data Type Implementation

## Overview
This project involves the creation of an abstract data type (ADT) called SymTable, which represents a symbol table. The SymTable ADT is designed to be generic, allowing values to be void pointers, thus capable of pointing to data of any type. Two implementations of the SymTable ADT are required: one using a linked list and the other using a hash table.

## Interface
The interface for the SymTable ADT is defined in the `symtable.h` file and includes the following functions:
- `SymTable_T SymTable_new(void)`
- `void SymTable_free(SymTable_T oSymTable)`
- `unsigned int SymTable_getLength(SymTable_T oSymTable)`
- `int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue)`
- `int SymTable_remove(SymTable_T oSymTable, const char *pcKey)`
- `int SymTable_contains(SymTable_T oSymTable, const char *pcKey)`
- `void *SymTable_get(SymTable_T oSymTable, const char *pcKey)`
- `void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),const void *pvExtra)`

## Linked List Implementation
The linked list implementation of SymTable is stored in the `symtablelist.c` file. It checks for checked runtime errors using the standard `assert` macro and ensures no memory leaks occur. Dynamic memory allocation is used, and all dynamically allocated memory is explicitly freed when no longer needed.

## Hash Table Implementation
The hash table implementation of SymTable is stored in the `symtablehash.c` file. It initially contains 509 buckets and uses a logical hash function. Similar to the linked list implementation, it checks for checked runtime errors, prevents memory leaks, and dynamically adjusts the number of buckets to ensure efficiency.

## Extra Credits
The SymTable can expand its size to accommodate a larger number of bindings. When the number of bindings exceeds certain thresholds, the number of buckets is increased accordingly. However, it stops expanding when reaching the maximum number of buckets (65521). While implementing expansion is optional, it is essential to ensure correctness over attempting expansion and failing. If expansion is not implemented, the README should describe the attempt made.


