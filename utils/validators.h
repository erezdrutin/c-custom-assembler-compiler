#ifndef MAMAN14_VALIDATORS_H
#define MAMAN14_VALIDATORS_H

#include "globals.h"
#include "../ds/issue.h"

int validate_struct_str(char *ptr);
int validate_str(const char *str, enum data_type dt);
enum data_type parse_str(char* str);
int validate_errors(issue *errors_arr, int ec);
#endif
