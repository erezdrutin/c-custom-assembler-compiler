#ifndef MAMAN14_FILES_H
#define MAMAN14_FILES_H
#include "../ds/word.h"
#include "./conversions.h"
#define OB_FILE_ROW_LEN 5
char* read_file(char* filename);
int write_file(char* filename, char* content);
int does_file_exist(const char *filename);
int write_file_special(char *filename, word *arr, unsigned long start, unsigned long len);
#endif //MAMAN14_FILES_H
