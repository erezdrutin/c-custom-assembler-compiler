#include "operators.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

operator ops[] = {
        {"mov", two_ops},
        {"cmp", two_ops},
        {"add", two_ops},
        {"sub", two_ops},
        {"lea", two_ops},
        {"not", one_op},
        {"clr", one_op},
        {"inc", one_op},
        {"dec", one_op},
        {"jmp", one_op},
        {"bne", one_op},
        {"get", one_op},
        {"prn", one_op},
        {"jsr", one_op},
        {"rts", zero_ops},
        {"hlt", zero_ops},
};


//const char *ops[16] = { "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
//                        "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};

/**
 * A function in charge of detecting an operator within the received string.
 * If any of the operators exist in the string, the index of the first matching operator will be returned.
 * @param str A string to search for ops in it.
 * @return The index of the relevant operator or -1 if there aren't any.
 */
int get_operator_index(char *str) {
    int len = (size_t) sizeof(ops) / sizeof(ops[0]), i;
    for (i = 0; i < len; ++i) {
        if (strstr(str, ops[i].name) || !strcmp(str, ops[i].name)) {
            return i;
        }
    }
    return -1;
}

/**
 * A function in charge of detecting an operator within the received string.
 * If any of the operators exist in the string, a pointer to the relevant operator will be returned.
 * @param str A string to search for ops in it.
 * @return A pointer to an operator struct (if there is a matching one, otherwise - NULL).
 */
operator * get_operator (char *str) {
    int len = (size_t) sizeof(ops) / sizeof(ops[0]), i;
    for (i = 0; i < len; ++i) {
        if (strstr(str, ops[i].name) || !strcmp(str, ops[i].name)) {
            return &ops[i];
        }
    }
    return NULL;
}

/**
 * A function in charge of validating the received operator's usage.
 * Basically checking if the amount of received parameters matches the amount of operands that are
 * associated with the received operator pointer.
 * @param str A string to validate.
 * @param op An operand to validate.
 * @return True (1) / False (0).
 */
int validate_operator_usage(const char *str) {
    // Duplicating the string & counting how many args were passed:
    char *ptr = strdup(str), *token = NULL;
    int args = 0, res;
    operator *temp = get_operator(ptr);
    // If we received an operator in which we expect to get 0 parameters:
    if (temp->ops_count == 0) {
        if (!strcmp(str, temp->name)) return 1;
        return 0;
    }
    // Fetching "first" part of string & Counting how many comma separated parts are in the string:
    token = strtok(ptr, ",");
    while( token != NULL ) {
        args++;
        token = strtok(NULL, " ");
    }

    free(ptr);
    // Validate that the amount of args matches our expectation + that the received string != the found operator:
    return args == temp->ops_count && strcmp((char *)str, temp->name) != 0;
}