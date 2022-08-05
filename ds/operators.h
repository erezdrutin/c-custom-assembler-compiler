#ifndef MAMAN14_OPERATORS_H
#define MAMAN14_OPERATORS_H
#include "word.h"
#include "issue.h"
#include "symbol.h"
#define REGISTERS_NUM 8
#define OPS_NUM 16
enum operands_count_options { zero_ops = 0, one_op = 1, two_ops = 2 };
enum addressing_methods {  immediate_addressing = 0, direct_addressing = 1, struct_addressing = 2, register_addressing = 3, invalid_addressing = -1};
enum operand_type { src_operand, dest_operand };

typedef struct Operator {
    char * name;
    enum operands_count_options ops_count;
} operator;

typedef struct OperatorMetaData {
    operator * op;
    enum addressing_methods src_am[4];
    enum addressing_methods dest_am[4];
} operatorMetaData;

extern operator ops[];
extern operatorMetaData opsMetaData[];
int get_operator_index(char *str);
operator * get_operator (char* str);
int validate_operator_usage_in_str(const char *str);
void encode_cmd(char *ptr, symbol ** head, unsigned long *pc, word **code_arr, issue ** errors_array, int *ec, int lc, enum run_type rt);
int is_register(char *str);
void handle_immediate_addressing(word **code_arr, unsigned long *pc, char *ptr, enum run_type rt);
void handle_register_addressing(word **code_arr, unsigned long *pc, const char *ptr, const char *ptr2, enum run_type rt);
void handle_struct_addressing(symbol * head, word **code_arr, unsigned long *pc, const char *str, enum run_type rt);
void handle_direct_addressing(symbol *head, word **code_arr, unsigned long *pc, char *ptr, enum run_type rt);
#endif //MAMAN14_OPERATORS_H
