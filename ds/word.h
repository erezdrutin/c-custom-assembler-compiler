#ifndef MAMAN14_WORD_H
#define MAMAN14_WORD_H
#include <stdlib.h>
#include "../utils/globals.h"
#define WORD_SIZE 11
typedef struct Word {
    char value[WORD_SIZE];
    unsigned int address;
} word;
void add_or_update_word_in_arr(word ** arr, unsigned long *counter, char * val, enum run_type rt);
word * append_word_arr(word **arr1, unsigned long counter1, word **arr2, unsigned long counter2);
#endif