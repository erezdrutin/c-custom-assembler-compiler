//
// Created by Erez on 29/07/2022.
//

#include "data.h"
#include <stdio.h>

void print_data_arr(data * arr, size_t dc) {
    size_t i;
    for (i = 0; i < dc; i++) {
        printf("value: %s, address: %u\n", arr[i].value, arr[i].address);
    }
}