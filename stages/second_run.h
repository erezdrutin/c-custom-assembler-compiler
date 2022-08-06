#ifndef MAMAN14_SECOND_RUN_H
#define MAMAN14_SECOND_RUN_H

#include "../ds/symbol.h"
#include "../ds/word.h"

void second_run(char *content, symbol **head, symbol **ent_table_head, word **mem_arr, unsigned long *mc,
                issue **errors_array, int *ec);
#endif
