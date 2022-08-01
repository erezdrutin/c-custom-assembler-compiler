//
// Created by Erez on 29/07/2022.
//

#ifndef MAMAN14_DATA_H
#define MAMAN14_DATA_H
#include <stdlib.h>
typedef struct Data {
    char value[11];
    unsigned int address;
} data;
void print_data_arr(data * arr, size_t dc);
#endif //MAMAN14_DATA_H
