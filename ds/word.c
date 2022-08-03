//
// Created by Erez on 29/07/2022.
//

#include "word.h"
#include <stdio.h>
#include <string.h>

void print_data_arr(word * arr, size_t dc) {
    size_t i;
    for (i = 0; i < dc; i++) {
        printf("value: %s, address: %u\n", arr[i].value, arr[i].address);
    }
}

 /**
  * A function in charge of dynamically allocating memory in order to append new words to the received array.
  * @param arr The array to which we would like to add a new word.
  * @param counter A counter that resembles the last index in the array.
  * @param val The value that we would like to append to the array.
  */
void add_new_word_to_arr(word ** arr, unsigned long *counter, char * val) {
    word new;
    strcpy(new.value, val);
    new.address = *counter;
    *arr = (word *) realloc(*arr, sizeof(word) * ((*counter) + 1));
    (*arr)[(*counter)++] = new;
}