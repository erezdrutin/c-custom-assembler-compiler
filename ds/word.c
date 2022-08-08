#include "word.h"
#include <string.h>

 /**
  * A function in charge of dynamically allocating memory in order to append new words to the received array.
  * If a certain word already exists at the received index (at the index *counter), we will update the word's value
  * if necessary.
  * @param arr The array to which we would like to add a new word.
  * @param counter A counter that resembles the last index in the array.
  * @param val The value that we would like to append to the array.
  * @param rt The run type (first / second).
  */
void add_or_update_word_in_arr(word ** arr, unsigned long *counter, char * val, enum run_type rt) {
    if (rt == first_run_type) {
        word new;
        strcpy(new.value, val);
        new.address = *counter;
        *arr = (word *) realloc(*arr, sizeof(word) * ((*counter) + 1));
        (*arr)[(*counter)++] = new;
    } else if (rt == second_run_type && strcmp((*arr)[(*counter)].value, val) != 0)
        strcpy((*arr)[(*counter)++].value, val);
    else
        (*counter)++;
}

/**
 * A function in charge of appending the 2nd words array to the first words array.
 * The function updates all the addresses of the 2nd array to "continue" the first array's addresses and
 * returns an array that contains the contents of both arrays.
 * @param arr1 The first array in the result.
 * @param counter1 A counter that indicates the amount of elements in the first array.
 * @param arr2 The second array in the result.
 * @param counter2 A counter that indicates the amount of elements in the second array.
 * @return An array that contains both first & second array.
 */
word * append_word_arr(word **arr1, unsigned long counter1, word **arr2, unsigned long counter2) {
    int i;
    word *new_arr = (word *)malloc((counter1 + counter2) * sizeof(word));
    /* Update addresses for all the 2nd array addresses: */
    for (i = 0; i < counter2; i++) (*arr2)[i].address += counter1;
    /* Copy first array, then copy 2nd array to new array: */
    memcpy(new_arr, *arr1, counter1 * sizeof(word));
    memcpy(new_arr + counter1, *arr2, counter2 * sizeof(word));
    /* Return new array: */
    return new_arr;
}