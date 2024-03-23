#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void pfApply(const char *pcKey, void *pvValue, void *pvExtra){
    printf("He\n");
}

int main(){
    SymTable_T tmp1;
    SymTable_T tmp2;
    tmp1 = SymTable_new();
    SymTable_put(tmp1, "hello", "12");
    SymTable_put(tmp1, "world", "34");
    SymTable_put(tmp1, "alex", "56");
    printf("%d\n", SymTable_getLength(tmp1));
    printf("%d\n", SymTable_contains(tmp1, "alex"));
    printf("%s\n", (const char*)SymTable_get(tmp1, "hello"));
    SymTable_remove(tmp1, "alex");
    printf("%d\n", SymTable_getLength(tmp1));
    SymTable_map(tmp1,pfApply,"3");
    SymTable_free(tmp1); 

    
    tmp2 = SymTable_new();
    SymTable_put(tmp2, "ALEXA", "43");
    SymTable_put(tmp2, "ALEXANDROS", "44");
    SymTable_put(tmp2, "ALEX", "45");
    printf("%d\n", SymTable_getLength(tmp2));
    printf("%d\n", SymTable_contains(tmp2, "ALEX"));
    printf("%s\n", (const char*)SymTable_get(tmp2, "ALEXANDROS"));
    SymTable_remove(tmp2, "ALEXA");
    printf("%d\n", SymTable_getLength(tmp2));
    SymTable_map(tmp2,pfApply,"43");
    SymTable_free(tmp2); 
    return 0;
}