#include "strings.h"
#include <stdlib.h>
#include <string.h>

/**
 * A function in charge of copying, concatenating and allocating memory for a string combined of 2 strings.
 * For example, if we were to pass str1 = "test", str2 = " hello", the function would:
 * 1. Allocate enough memory to store the concatenated string.
 * 2. Copy both strings one after the other.
 * 3. Return a pointer to the new "merged" string.
 * @param str1 - First string in the result.
 * @param str2 - Second string in the result.
 * @return A new string combined from both received strings.
 */
char* str_cat_copy(const char *str1, const char *str2) {
    unsigned long str1_len, str2_len;
    char *new_str;

    /* If we received 2 empty strings - return null:*/
    if (str1 == NULL && str2 == NULL)
        return NULL;

    str1_len = strlen(str1);
    str2_len = strlen(str2);

    new_str = malloc(str1_len + str2_len + 1);

    /* If we fail to allocate memory for some reason - return null: */
    if (new_str == NULL)
        return NULL;

    memcpy(new_str, str1, str1_len);
    memcpy(new_str + str1_len, str2, str2_len + 1);

    return new_str;
}