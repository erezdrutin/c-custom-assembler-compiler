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

int append_unique(symbol** head_ref, symbol** ent_table_head, char* new_symbol, unsigned int new_address, enum symbol_type new_kind,
                  issue **errors_array, int * ec, int lc);
void append_unique_symbol_from_op(symbol** head_ref, symbol** ent_table_head, char* new_symbol, unsigned int new_address, enum symbol_type new_kind,
                                 issue **errors_array, int *ec, int lc);
symbol * search_list(symbol * ptr, char * value);
void update_list_addresses(const symbol * node, unsigned int n, enum symbol_type st);
symbol * get_extern_symbols_from_ll(const symbol *node);
symbol * get_entry_symbols_from_ll(const symbol *node);
symbol *generate_symbols_table(const symbol *head, const symbol *ent_table_head);
void free_list(symbol *head);
enum symbol_type get_symbol_type(const char *str);
#endif
