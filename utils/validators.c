#include "validators.h"
#include "string_utils.h"
#include "../ds/operators.h"
#include <string.h>
#include <stdio.h>

/**
 * A function in charge of validating a struct string.
 * True represents that the string is valid & False represents invalid.
 * @param ptr A pointer to a string.
 * @param dt The datatype associated with the string.
 * @return True (1) / False (0).
 */
int validate_struct_str(char *ptr) {
    // Skipping the ".struct " part:
    char *temp = strdup(trim(ptr) + strlen(".struct") + 1);
    ptr = temp;
    char *token = strtok(temp, ",");
    // First parameter must be a number:
    if (!is_number(trim(token)))
        return 0;
    token = strtok(NULL, ",");
    // Second parameter must be a valid string (looks like "<str>"):
    if (count_char_in_string(token, '"') != 2)
        return 0;
    // Freeing the memory & returning valid:
    free(ptr);
    return 1;
}

/**
 * A function in charge of validating the each of the received detected data types match a valid line.
 * If the received string is invalid, 0 (False) will be returned.
 * @param str A string to validate.
 * @param dt Datatype that we detected as associated with the string.
 * @return True (1) / False (0).
 */
int validate_str(const char *str, enum data_type dt) {
    char *ptr = (char *)str;
    if (dt == sdata && strcmp(str, ".data") != 0 && count_numbers_in_string(str) > 0) return 1;
    if (dt == sstring && count_char_in_string(str, '"') == 2) return 1;
    if (dt == sstruct) return validate_struct_str(ptr);
    if (dt == iextern && count_char_in_string(trim(ptr), ' ') == 1) return 1;
    if (dt == ientry && count_char_in_string(trim(ptr), ' ') == 1) return 1;
    // We're handling this part later:
    if (dt == op) return 1;
    return 0;
}

enum data_type parse_str(char* str) {
    if (strchr(str, ':')) return symbol_dt;
    if (strstr(str, ".data")) return sdata;
    if (strstr(str, ".string")) return sstring;
    if (strstr(str, ".struct")) return sstruct;
    if (strstr(str, ".extern")) return iextern;
    if (strstr(str, ".entry")) return ientry;
    if (get_operator(str) != NULL) return op;
    return invalid;
}


int validate_errors(issue *errors_arr, int ec) {
    if (ec != 0) {
        printf("\n\n\n~~~~~~~~~~~~~~~~ERRORS:~~~~~~~~~~~~~~~~\n");
        print_issues(errors_arr, ec);
        return 0;
    }
    return 1;
}