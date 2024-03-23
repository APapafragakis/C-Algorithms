/**
 * Alexandros Papafragakis
 * AM: 5084
 * CS-255: Assignment 2
 * @file symtable.h
 * @brief This file contains the interface of the SymTable ADT.
 * @author Alexandros Papafragakis
*/

/**
 * @typedef SymTable_T
 * @brief Define a type for the symbol table.
 */
typedef struct SymTable *SymTable_T;


/**
 * @brief This function creates a new symbol table.
 * @return A pointer to the newly created symbol table.
 */
SymTable_T SymTable_new(void);

/**
 * @brief This function frees the memory allocated for a symbol table.
 * @param oSymTable The symbol table to be freed.
 */
void SymTable_free(SymTable_T oSymTable);

/**
 * @brief This function returns the number of key-value pairs in the symbol table.
 * @param oSymTable The symbol table.
 * @return The number of key-value pairs in the symbol table.
 */
unsigned int SymTable_getLength(SymTable_T oSymTable);

/**
 * @brief This function inserts a new key-value pair into the symbol table.
 * @param oSymTable The symbol table.
 * @param pcKey The key to be inserted.
 * @param pvValue The value to be associated with the key.
 * @return 1 if the operation is successful, 0 otherwise.
 */
int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue);

/**
 * @brief This function removes a key-value pair from the symbol table.
 * @param oSymTable The symbol table.
 * @param pcKey The key to be removed.
 * @return 1 if the operation is successful, 0 otherwise.
 */
int SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/**
 * @brief This function checks if a key is in the symbol table.
 * @param oSymTable The symbol table.
 * @param pcKey The key to be checked.
 * @return 1 if the key is in the symbol table, 0 otherwise.
 */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/**
 * @brief This function retrieves the value associated with a key in the symbol table.
 * @param oSymTable The symbol table.
 * @param pcKey The key to be searched for.
 * @return The value associated with the key if it is in the symbol table, NULL otherwise.
 */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/**
 * @brief This function applies a function to each key-value pair in the symbol table.
 * @param oSymTable The symbol table.
 * @param pfApply The function to be applied.
 * @param pvExtra An extra parameter that can be passed to the function.
 */
void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),const void *pvExtra);