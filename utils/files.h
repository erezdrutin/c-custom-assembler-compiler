#ifndef MAMAN14_FILES_H
#define MAMAN14_FILES_H
#include "../ds/word.h"
#include "../ds/symbol.h"
#include "./conversions.h"
// The length of a line in the .ob file:
#define OB_FILE_ROW_LEN 5
// The added length to store the base 32 representation of a symbol:
#define CUSTOM_FILE_ADD_LEN 3
char* read_file(char* filename);
int write_file(char* filename, char* content);
int does_file_exist(const char *filename);
int write_file_custom_word_arr(char *filename, word *arr, unsigned long start, unsigned long len);
int write_file_custom_symbols_ll(char *filename, symbol *ptr);
#endif
