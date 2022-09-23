#include "validators.h"
#include "string_utils.h"
#include "operators.h"
#include <string.h>

/**
 * A function in charge of validating a struct string.
 * True represents that the string is valid & False represents invalid.
 * @param ptr A pointer to a string.
 * @param dt The datatype associated with the string.
 * @return True (1) / False (0).
 */
int validate_struct_str(char *ptr) {
    /* Skipping the ".struct " part: */
    char *temp = strdup(trim(ptr) + strlen(".struct") + 1);
    ptr = temp;
    char *token = strtok(temp, ",");
    /* First parameter must be a number: */
    if (!is_number(trim(token)))
        return 0;
    token = strtok(NULL, ",");
    /* Second parameter must be a valid string (looks like "<str>"): */
    if (count_char_in_string(token, '"') != 2)
        return 0;
    /* Freeing the memory & returning valid: */
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
    /* We're handling this part later: */
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


/**
 * A function in charge of validating the received operator's usage.
 * Basically checking if the amount of received parameters matches the amount of operands that are
 * associated with the received operator pointer.
 * @param str A string to validate.
 * @param op An operand to validate.
 * @return True (1) / False (0).
 */
int validate_operator_usage_in_str(const char *str) {
    /* Duplicating the string & counting how many args were passed: */
    char *ptr = trim((char *)str), *token = NULL;
    int args = 0, res;
    operator *temp = get_operator(ptr);
    /* If we received an operator in which we expect to get 0 parameters: */
    if (temp->ops_count == 0) {
        if (!strcmp(ptr, temp->name)) return 1;
        return 0;
    }
    /* Fetching "first" part of string & Counting how many comma separated parts are in the string: */
    ptr = strdup(trim(ptr + strlen(temp->name)));
    token = strtok(ptr, ",");
    if (temp -> ops_count == 1 && token != NULL) args++;
    while( temp->ops_count == 2 && token != NULL) {
        args++;
        token = strtok(NULL, " ");
    }

    free(ptr);
    /* Validate that the amount of args matches our expectation + that the received string != the found operator: */
    return args == temp->ops_count;
}


/**
 * A function in charge of validating the addressing methods used when using a certain operator.
 * @param src_am The addressing method for the src operand.
 * @param dest_am The addressing method for the dest operand.
 * @param op_md The operator metadata struct.
 * @return True (1) if the addressing methods are valid or False (0) if not.
 */
int validate_operator_am_usage(enum addressing_methods src_am, enum addressing_methods dest_am, operatorMetaData *op_md) {
    int i, src_flag = 0, dest_flag = 0;
    for (i = 0; i < 4 && !(src_flag && dest_flag); i++) {
        /* Validating addressing methods for both src & dest: */
        if (op_md->src_am[i] == src_am)
            src_flag = 1;
        if (op_md->dest_am[i] == dest_am)
            dest_flag = 1;
        /* If we expected an invalid addressing method but received another addressing method - return 0
         * (initializing src with 0 is enough in this case): */
        if ((op_md->src_am[i] == invalid_addressing && src_am != invalid_addressing) || (op_md->dest_am[i] == invalid_addressing && dest_am != invalid_addressing)) {
            src_flag = 0;
            break;
        }
    }
    return src_flag && dest_flag;
}
