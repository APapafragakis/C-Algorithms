/**
 * Alexandros Papafragakis
 * AM: 5084
 * CS-255: Assignment 2
 * @file symtablehash.c
 * @brief This file contains the implementation of the SymTable ADT using a hash table.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symtable.h"

#define HASH_MULTIPLIER 65599
#define CAPACITY 509

/**
 * @struct Binding
 * @brief A structure representing a key-value pair in the symbol table.
 */
struct Binding{
	char *key;
	void *value;
    struct Binding *next;
};


struct SymTable {
    struct Binding** table;
    int length;
};

/* Return a hash code for pcKey. */
static unsigned int SymTable_hash(const char *pcKey){
	size_t ui;
	unsigned int uiHash = 0U;
    for (ui = 0U; pcKey[ui] != '\0'; ui++){
        uiHash = uiHash * HASH_MULTIPLIER + pcKey[ui];
    }
	return uiHash % CAPACITY;
}

SymTable_T SymTable_new(void){
    SymTable_T oSymTable;
    int i;
    oSymTable = malloc(sizeof(struct SymTable));
    oSymTable -> length = 0;
    oSymTable -> table = malloc(sizeof(struct Binding*)*CAPACITY);
    for(i = 0; i<CAPACITY; i++){
        oSymTable -> table[i] = NULL;
    }
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable){
    int i;
    struct Binding *prev, *tmp_binding;
    assert(oSymTable);
    for(i = 0; i<CAPACITY; i++){
        prev = oSymTable -> table[i];
        tmp_binding = oSymTable -> table[i];
        while(prev != NULL){
	        tmp_binding = prev;
	        prev = prev -> next;
	        free(tmp_binding);
        }
    }
    oSymTable -> length = 0;
    free(oSymTable);
}

unsigned int SymTable_getLength(SymTable_T oSymTable){
    assert(oSymTable);
    return oSymTable -> length;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue){
    int pos =  SymTable_hash(pcKey);
    struct Binding* tmp = malloc(sizeof(struct Binding));
    assert(pcKey);
    assert(oSymTable);
    tmp -> value = (void*)pvValue;
    if(SymTable_contains(oSymTable,pcKey) == 1){ 
	    free(tmp);
        return 0;
    }
    tmp -> key = malloc(strlen(pcKey)+1);
    strcpy(tmp -> key,pcKey);
    tmp -> next = oSymTable -> table[pos];
    oSymTable -> table[pos] = tmp;
    oSymTable -> length++;
    return 1;
}

int SymTable_remove(SymTable_T oSymTable, const char *pcKey){
    struct Binding *tmp, *prev;
    int pos = SymTable_hash(pcKey);
    tmp = oSymTable -> table[pos];
    prev = NULL;
    assert(oSymTable);
    assert(pcKey);
    while(tmp!= NULL){
        if(strcmp(tmp->key,pcKey) == 0){
            if(prev == NULL){
                oSymTable -> table[pos] = tmp -> next;
            } else {
                prev -> next = tmp -> next;
            }
            free(tmp -> key);
            free(tmp);
            oSymTable -> length--;
            return 1;
        }
        prev = tmp;
        tmp = tmp -> next;
    }
    return 0;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey){
    int pos =  SymTable_hash(pcKey);
    struct Binding *tmp;
    assert(pcKey);
    assert(oSymTable);
    tmp = oSymTable -> table[pos];
    while (tmp != NULL){
        if (!strcmp(tmp->key,pcKey)){
            return 1;
        }
        tmp = tmp->next;
    }
    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
    int pos = SymTable_hash(pcKey);
    struct Binding *tmp;
    assert(oSymTable);
    assert(pcKey);
    tmp = oSymTable->table[pos];
    while (tmp != NULL){
        if (!strcmp(tmp->key,pcKey)){
			return tmp->value;
		}
        tmp = tmp->next;
    }
	return NULL;
}

void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),const void *pvExtra){
    struct Binding *tmp;
    int i = 0;
    assert(oSymTable);
    assert(pfApply);
    for (i = 0; i < oSymTable->length; i++) {
        tmp = oSymTable->table[i];
        while (tmp != NULL) {
            pfApply(tmp->key, tmp->value, (void *)pvExtra);
            tmp = tmp->next;
        }
    }
}