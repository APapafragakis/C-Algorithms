/**
 * Alexandros Papafragakis
 * AM: 5084
 * CS-255: Assignment 2
 * @file symtablelist.c
 * @brief This file contains the implementation of the SymTable ADT using a linked list.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symtable.h"

/**
 * @struct Binding
 * @brief A structure representing a key-value pair in the symbol table.
 */
struct Binding{
	char *key;
	void *value;
    struct Binding *next;
};


struct SymTable{
	int length;
	struct Binding *head;
};

SymTable_T SymTable_new(void){
	SymTable_T oSymTable;
	oSymTable = malloc(sizeof(struct SymTable));
	oSymTable-> length = 0;
	oSymTable-> head = NULL;
	return oSymTable;
}

void SymTable_free(SymTable_T oSymTable){
    struct Binding *prev_binding, *tmp_binding;
    assert(oSymTable);
    prev_binding = oSymTable -> head;
    tmp_binding = oSymTable -> head;
    while(prev_binding != NULL){
	    tmp_binding = prev_binding;
	    prev_binding = prev_binding->next;
	    free(tmp_binding);
    }
    oSymTable -> head = NULL;
    oSymTable -> length = 0;
    free(oSymTable);
}

unsigned int SymTable_getLength(SymTable_T oSymTable){
    assert(oSymTable);
	return oSymTable->length;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue){
	struct Binding *tmp;
    assert(oSymTable);
    assert(pcKey);
    if(SymTable_contains(oSymTable,pcKey) == 1){ 
	    return 0;
    }
    tmp = malloc(sizeof(struct Binding)); 
    tmp -> key = malloc((strlen(pcKey)+1)*sizeof(char));
    strcpy(tmp -> key, pcKey);
    tmp->value = (void*)pvValue;
    tmp->next = oSymTable -> head;
    oSymTable -> head = tmp;
    oSymTable -> length++;
    return 1;
}

int SymTable_remove(SymTable_T oSymTable, const char *pcKey){
    struct Binding *tmp, *prev;
    assert(oSymTable);
    assert(pcKey);
	tmp = oSymTable -> head;
    prev = NULL;
    while (tmp != NULL){
        if((!strcmp(tmp->key,pcKey))){
			if (prev == NULL){
				oSymTable->head = oSymTable -> head -> next;
				free(tmp -> key);
				free(tmp);
			}
			else {
				prev->next = tmp->next;
				free(tmp->key);
				free(tmp);
			}
            oSymTable->length--;
            return 1;
		}
        prev = tmp;
        tmp = tmp -> next;
    }
	return 0;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey){
    struct Binding *tmp;
    assert(oSymTable);
    assert(pcKey);
	tmp = oSymTable->head;
	while (tmp != NULL){
        if (!strcmp(tmp->key,pcKey)){
             return 1;
        }
        tmp = tmp->next;
    }
	return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
    struct Binding *tmp;
    assert(oSymTable);
    assert(pcKey);
    tmp = oSymTable->head;
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
    assert(oSymTable);
    assert(pfApply);
    tmp = oSymTable -> head;
    while(tmp != NULL){
        pfApply (tmp -> key, tmp -> value, (void*)pvExtra);
        tmp = tmp -> next;
    }
}
