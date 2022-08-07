#ifndef MAMAN14_OPERATORS_H
#define MAMAN14_OPERATORS_H
#include "word.h"
#include "issue.h"
#include "symbol.h"
#include "../utils/globals.h"
#define REGISTERS_NUM 8
#define OPS_NUM 16

extern operator ops[];
extern operatorMetaData opsMetaData[];

int get_operator_index(char *str);
operator * get_operator (char* str);
void encode_cmd(char *ptr, symbol ** head, symbol **ent_table_head, unsigned long *pc, word **code_arr, issue ** errors_array, int *ec, int lc, enum run_type rt);

#endif
