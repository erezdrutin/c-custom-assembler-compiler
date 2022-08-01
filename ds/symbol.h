#ifndef MAMAN14_SYMBOL_H
#define MAMAN14_SYMBOL_H
#include <stdlib.h>
#include "../ds/issue.h"
enum symbol_type { symbol_data, symbol_code, symbol_invalid };
typedef struct Node
{
    char* value;
    unsigned int address;
    enum symbol_type kind;
    struct Node *next;
} symbol;
void push(symbol** head_ref, char* new_symbol, unsigned int new_address, enum symbol_type new_kind);
void insertAfter(symbol* prev_node, char* new_symbol, unsigned int new_address, enum symbol_type new_kind);
void append(symbol** head_ref, char* new_symbol, unsigned int new_address, enum symbol_type new_kind);
int append_unique(symbol** head_ref, char* new_symbol, unsigned int new_address, enum symbol_type new_kind,
                   issue **errors_array, int * ec, int lc);
void printList(symbol *node);
#endif //MAMAN14_SYMBOL_H
