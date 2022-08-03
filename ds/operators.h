#ifndef MAMAN14_OPERATORS_H
#define MAMAN14_OPERATORS_H
#include "word.h"
#include "issue.h"
#include "symbol.h"
#define REGISTERS_NUM 8
#define OPS_NUM 16
enum operands_count_options { zero_ops = 0, one_op = 1, two_ops = 2 };
enum addressing_methods {  immediate_addressing = 0, direct_addressing = 1, struct_addressing = 2, register_addressing = 3, invalid_addressing = -1};

typedef struct Operator {
    char * name;
    enum operands_count_options ops_count;
} operator;
extern operator ops[];
int get_operator_index(char *str);
operator * get_operator (char* str);
int validate_operator_usage(const char *str);
void perform_mov_op(char *ptr, symbol ** head, unsigned long *pc, word **code_arr, issue ** errors_array, int *ec, int lc);
int is_register(char *str);
void handle_immediate_addressing(word **code_arr, unsigned long *pc, char *ptr);
void handle_register_addressing(word **code_arr, unsigned long *pc, const char *ptr, const char *ptr2);
void handle_struct_addressing(symbol * head, word **code_arr, unsigned long *pc, const char *str);
void handle_direct_addressing(symbol *head, word **code_arr, unsigned long *pc, char *ptr);
#endif //MAMAN14_OPERATORS_H
