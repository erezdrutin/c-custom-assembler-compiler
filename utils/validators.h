#ifndef MAMAN14_VALIDATORS_H
#define MAMAN14_VALIDATORS_H

#include "globals.h"

int validate_struct_str(char *ptr);
int validate_str(const char *str, enum data_type dt);
enum data_type parse_str(char* str);
int validate_operator_usage_in_str(const char *str);
int validate_operator_am_usage(enum addressing_methods src_am, enum addressing_methods dest_am, operatorMetaData *op_md);
#endif
