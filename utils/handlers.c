#include "handlers.h"
#include "../ds/operators.h"
#include "string_utils.h"
#include "conversions.h"
#include "validators.h"
#include <string.h>
#include <ctype.h>
#include <printf.h>

/**
 * Parsing every piece of data from the received data line.
 * For each data, we will generate assign a matching data struct & address in memory, while updating dc's value
 * accordingly at the same time.
 * @param ptr A pointer to a data line.
 * @param dc The data counter.
 * @param data_arr An array of data structs.
 */
void handle_data(char *ptr, unsigned long *dc, word **data_arr, enum run_type rt) {
    while (*ptr) { // While there are more characters to process...
        if ( isdigit(*ptr) || ( (*ptr=='-'||*ptr=='+') && isdigit(*(ptr+1)) )) {
            if (rt == second_run_type) (*dc)++;
            else {
                word temp;
                convert_to_10_bit_bin(strtoll(ptr, &ptr, 10), temp.value);
                temp.address = (*dc);
                *data_arr = (word *)realloc(*data_arr, sizeof(word) * ((*dc) + 1));
                (*data_arr)[(*dc)++] = temp;
            }
            ptr = strchr(ptr, ',') ? strchr(ptr, ',') : strchr(ptr, '\0');
        } else {
            ptr++;
        }
    }
}

/**
 * Parsing every piece of string from the received string line.
 * For each character in the string, we will generate assign a matching struct & address in memory,
 * while updating dc's value accordingly at the same time.
 * @param ptr A pointer to string line.
 * @param dc The counter of the data.
 * @param data_arr An array of data to which we would like to append stuff if relevant.
 */
void handle_string(char *ptr, unsigned long *dc, word **data_arr, enum run_type rt) {
    ptr = strchr(ptr, '"') + 1;
    while (*ptr != '"' && *ptr) {
        if (isalnum(*ptr)) {
            if (rt == second_run_type) (*dc)++;
            else {
                word temp;
                convert_to_10_bit_bin((size_t)*ptr, temp.value);
                temp.address = (*dc);
                *data_arr = (word *)realloc(*data_arr, sizeof(word) * ((*dc) + 1));
                (*data_arr)[(*dc)++] = temp;
            }
        }
        ptr++;
    }

    // Inserting an extra word to the data array that resembles the end of the string:
    if (rt == second_run_type) {
        (*dc)++;
        return;
    }

    word temp;
    convert_to_10_bit_bin(0, temp.value);
    temp.address = (*dc);
    *data_arr = (word *)realloc(*data_arr, sizeof(word) * ((*dc) + 1));
    (*data_arr)[(*dc)++] = temp;
}


// Check for invalid usage of oprator (i.e
void handle_operator(char *ptr, symbol **head, symbol **ent_table_head, unsigned long *pc, word **code_arr, issue ** errors_array, int *ec, int lc, enum run_type rt) {
    if (!validate_operator_usage_in_str(ptr)) {
        // ops_count is a single digit number, meaning it will have 1 char, therefore it will replace the "%d" and we
        // don't need to allocate any extra memory (+1 for \0):
        size_t errLen = strlen("Operator received wrong amount of args - Expected: %d.");
        char *error = (char *)malloc((strlen("Operator received wrong amount of args - Expected: %d."))* sizeof(char));
        snprintf(error, errLen, "Operator received wrong amount of args - Expected: %d.", (int)get_operator(ptr)->ops_count);
        add_new_issue_to_arr(errors_array, ec, lc, error);
        return;
    }
    // In any other case - encode the command and store it in the code array:
    encode_cmd(ptr, head, ent_table_head, pc, code_arr, errors_array, ec, lc, rt);
}


/**
 * Handle entry lines from the source code.
 * Basically detects and appends entry symbols to the symbols linked list.
 * @param head A pointer to the head of a linked list that consists of symbols.
 * @param ent_table_head A pointer to the head of a ll that consists of entry symbols.
 * @param ptr A pointer to a given line from the source code.
 * @param errors_array An array of errors which will be outputted to the user.
 * @param ec errors counter (index in errors array).
 * @param lc lines counter (lines index in source code file).
 */
void handle_entry(symbol** head, symbol **ent_table_head, char *ptr, issue ** errors_array, int *ec, int lc) {
    // Skip the extern word & define a token with the contents of the first symbol:
    ptr = ptr + strlen(".entry") + 1;
    char *symbolName = NULL, *token = strtok(ptr, ",");
    // Appending each symbol to the symbols list:
    while (token != NULL) {
        symbolName = (char *)strdup(token);
        append_unique(head, ent_table_head, symbolName, 0, symbol_entry, errors_array, ec, lc);
        token = strtok(NULL, ",");
    }
}

/**
 * Handle extern lines from the source code.
 * Basically detects and appends extern symbols to the symbols linked list.
 * @param head A pointer to the head of a linked list that consists of symbols.
 * @param ptr A pointer to a given line from the source code.
 * @param errors_array An array of errors which will be outputted to the user.
 * @param ec errors counter (index in errors array).
 * @param lc lines counter (lines index in source code file).
 */
void handle_extern(symbol** head, symbol **ent_table_head, char *ptr, issue ** errors_array, int *ec, int lc) {
    // Skip the extern word & define a token with the contents of the first symbol:
    ptr = ptr + strlen(".extern") + 1;
    char *symbolName = NULL, *token = strtok(ptr, ",");
    // Appending each symbol to the symbols list:
    while (token != NULL) {
        symbolName = (char *)strdup(token);
        append_unique(head, ent_table_head, symbolName, 0, symbol_extern, errors_array, ec, lc);
        token = strtok(NULL, ",");
    }
}

int handle_append_symbol_to_symbols_arr(symbol** head, symbol **ent_table_head, unsigned long pc, unsigned long dc, char **ptr, issue ** errors_array, int *ec, int lc, char *symbolName) {
    enum symbol_type st = get_symbol_type(*ptr);
    if (st == symbol_data)
        return append_unique(head, ent_table_head, symbolName, dc, symbol_data, errors_array, ec, lc);
    else if (st == symbol_code)
        return append_unique(head, ent_table_head, symbolName, pc, symbol_code, errors_array, ec, lc);
    else if (st == symbol_entry)
        return append_unique(head, ent_table_head, symbolName, 0, symbol_entry, errors_array, ec, lc);
    else if (st == symbol_extern)
        return append_unique(head, ent_table_head, symbolName, 0, symbol_extern, errors_array, ec, lc);
    return 0;
}

/**
 * A function in charge of handling a case in which the received string contains a symbol_type.
 * @param head The head of the symbols linked list.
 * @param dc The counter of the data.
 * @param ptr A pointer to the current line.
 */
int handle_symbol(symbol** head, symbol **ent_table_head, unsigned long pc, unsigned long dc, char **ptr, issue ** errors_array, int *ec, int lc) {
    unsigned short symbolLen;
    char *symbolName = NULL;
    // Checking if the current line contains a symbol_type or not:

    if (parse_str(*ptr) == symbol_dt) {
        // Allocating memory to store the symbol_type & adding it to the symbols list:
        symbolLen = (unsigned short)(strchr(*ptr, ':') - *ptr);
        symbolName = (char *)strndup(*ptr, symbolLen);
        *ptr += symbolLen + 1;
        return handle_append_symbol_to_symbols_arr(head, ent_table_head, pc, dc, ptr, errors_array, ec, lc, symbolName);
    }
    return 1;
}