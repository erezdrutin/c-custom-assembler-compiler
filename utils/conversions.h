//
// Created by Erez on 29/07/2022.
//

#ifndef MAMAN14_CONVERSIONS_H
#define MAMAN14_CONVERSIONS_H
#include <stdlib.h>
char * convert_to_custom_base(size_t n);
void convert_to_10_bit_bin(size_t number, char * value);
int convert_10_bit_bin_to_decimal(const char *binary);
#endif //MAMAN14_CONVERSIONS_H