#ifndef MAMAN14_SYMBOL_H
#define MAMAN14_SYMBOL_H
#include <stdlib.h>
#include "../ds/issue.h"
#include "../utils/globals.h"
enum symbol_type { symbol_data, symbol_code, symbol_extern, symbol_entry, symbol_invalid };
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
int append_unique(symbol** head_ref, symbol** ent_table_head, char* new_symbol, unsigned int new_address, enum symbol_type new_kind,
                  issue **errors_array, int * ec, int lc);
int append_unique_symbol_from_op(symbol** head_ref, symbol** ent_table_head, char* new_symbol, unsigned int new_address, enum symbol_type new_kind,
                                 issue **errors_array, int *ec, int lc);
        void printList(const struct Node *node);
int list_exists(const symbol * ptr, char * value);
int list_exists_with_type(const symbol *ptr, char *value, enum symbol_type st);
symbol * search_list(symbol * ptr, char * value);
symbol * search_list_with_type(symbol * ptr, char * value, enum symbol_type st);
symbol * search_list_for_type(symbol * ptr, enum symbol_type st);
symbol *search_list_without_type(symbol * ptr, char * value, enum symbol_type st);
void update_list_addresses(const symbol * node, unsigned int n, enum symbol_type st);
void update_list_custom_type(const symbol * node, enum symbol_type st);
symbol * get_extern_symbols_from_ll(const symbol *node);
symbol * get_entry_symbols_from_ll(const symbol *node);
symbol *generate_symbols_table(const symbol *head, const symbol *ent_table_head);
void free_list(symbol *head);
#endif //MAMAN14_SYMBOL_H
