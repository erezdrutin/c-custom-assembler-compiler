#ifndef MAMAN14_ADDR_METHODS_H
#define MAMAN14_ADDR_METHODS_H
#include "word.h"
#include "symbol.h"
#include "operators.h"
#include "globals.h"


int is_label(char *str);
int is_register(char *str);
int is_struct_access_call(const char *str);
enum addressing_methods determine_addressing_method(char *str);
void handle_immediate_addressing(word **code_arr, unsigned long *pc, char *ptr, enum run_type rt);
void handle_register_addressing(word **code_arr, unsigned long *pc, const char *ptr, const char *ptr2, enum run_type rt);
void handle_struct_addressing(symbol * head, symbol *ent_table_head, word **code_arr, unsigned long *pc, const char *str, enum run_type rt, issue ** errors_arr, int *ec, int lc);
void handle_direct_addressing(symbol *head, symbol *ent_table_head, word **code_arr, unsigned long *pc, char *ptr, enum run_type rt, issue ** errors_arr, int *ec, int lc);
void handle_addressing_method(symbol ** head, symbol **ent_table_head, unsigned long *pc, word **code_arr, char *ptr, enum addressing_methods method, enum operand_type ot, enum run_type rt, issue ** errors_array, int *ec, int lc);
#endif
