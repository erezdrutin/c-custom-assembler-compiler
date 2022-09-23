#ifndef MAMAN14_FIRST_RUN_H
#define MAMAN14_FIRST_RUN_H

#include "symbol.h"
#include "word.h"

void first_run(const char *content, symbol **head, symbol **ent_table_head, unsigned long *pc, word **code_arr, unsigned long *dc, word **data_arr, issue **errors_array, int *ec);
#endif
