#ifndef MAMAN14_HANDLERS_H
#define MAMAN14_HANDLERS_H

#include "../ds/word.h"
#include "../ds/issue.h"
#include "../ds/symbol.h"

void handle_data(char *ptr, unsigned long *dc, word **data_arr, enum run_type rt);
void handle_string(char *ptr, unsigned long *dc, word **data_arr, enum run_type rt);
void handle_operator(char *ptr, symbol **head, symbol **ent_table_head, unsigned long *pc,
                     word **code_arr, issue ** errors_array, int *ec, int lc, enum run_type rt);
void handle_entry(symbol** head, symbol **ent_table_head, char *ptr, issue ** errors_array, int *ec, int lc);
void handle_extern(symbol** head, symbol **ent_table_head, char *ptr, issue ** errors_array, int *ec, int lc);
int handle_append_symbol_to_symbols_arr(symbol** head, symbol **ent_table_head, unsigned long pc, unsigned long dc,
                                        char **ptr, issue ** errors_array, int *ec, int lc, char *symbolName);
int handle_symbol(symbol** head, symbol **ent_table_head, unsigned long pc, unsigned long dc, char **ptr,
                  issue ** errors_array, int *ec, int lc);
#endif
