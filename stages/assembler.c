#include "assembler.h"
#include "first_run.h"
#include "second_run.h"
#include "../ds/word.h"
#include "../ds/operators.h"
#include "../utils/conversions.h"
#include "../utils/validators.h"
#include "../utils/string_utils.h"
#include <string.h>
#include <ctype.h>
#include <printf.h>

//enum data_type { symbol_dt, sdata, sstring, sstruct, iextern, ientry, op, invalid };
//
///**
// * A function in charge of validating a struct string.
// * True represents that the string is valid & False represents invalid.
// * @param ptr A pointer to a string.
// * @param dt The datatype associated with the string.
// * @return True (1) / False (0).
// */
//int validate_struct_str(char *ptr, enum data_type dt) {
//    // Skipping the ".struct " part:
//    char *temp = strdup(trim(ptr) + strlen(".struct") + 1);
//    ptr = temp;
//    char *token = strtok(temp, ",");
//    // First parameter must be a number:
//    if (!is_number(trim(token)))
//        return 0;
//    token = strtok(NULL, ",");
//    // Second parameter must be a valid string (looks like "<str>"):
//    if (count_char_in_string(token, '"') != 2)
//        return 0;
//    // Freeing the memory & returning valid:
//    free(ptr);
//    return 1;
//}
//
///**
// * A function in charge of validating the each of the received detected data types match a valid line.
// * If the received string is invalid, 0 (False) will be returned.
// * @param str A string to validate.
// * @param dt Datatype that we detected as associated with the string.
// * @return True (1) / False (0).
// */
//int validate_str(const char *str, enum data_type dt) {
//    char *ptr = (char *)str;
//    if (dt == sdata && strcmp(str, ".data") != 0 && count_numbers_in_string(str) > 0) return 1;
//    if (dt == sstring && count_char_in_string(str, '"') == 2) return 1;
//    if (dt == sstruct) return validate_struct_str(ptr, dt);
//    if (dt == iextern && count_char_in_string(trim(ptr), ' ') == 1) return 1;
//    if (dt == ientry && count_char_in_string(trim(ptr), ' ') == 1) return 1;
//    // We're handling this part later:
//    if (dt == op) return 1;
//    return 0;
//}
//
//enum data_type parse_str(char* str) {
//    if (strchr(str, ':')) return symbol_dt;
//    if (strstr(str, ".data")) return sdata;
//    if (strstr(str, ".string")) return sstring;
//    if (strstr(str, ".struct")) return sstruct;
//    if (strstr(str, ".extern")) return iextern;
//    if (strstr(str, ".entry")) return ientry;
//    if (get_operator(str) != NULL) return op;
//    return invalid;
//}
//
///**
// * Parsing every piece of data from the received data line.
// * For each data, we will generate assign a matching data struct & address in memory, while updating dc's value
// * accordingly at the same time.
// * @param ptr A pointer to a data line.
// * @param dc The data counter.
// * @param data_arr An array of data structs.
// */
//void handle_data(char *ptr, unsigned long *dc, word **data_arr, enum run_type rt) {
//    while (*ptr) { // While there are more characters to process...
//        if ( isdigit(*ptr) || ( (*ptr=='-'||*ptr=='+') && isdigit(*(ptr+1)) )) {
//            if (rt == second_run_type) (*dc)++;
//            else {
//                word temp;
//                convert_to_10_bit_bin(strtoll(ptr, &ptr, 10), temp.value);
//                temp.address = (*dc);
//                *data_arr = (word *)realloc(*data_arr, sizeof(word) * ((*dc) + 1));
//                (*data_arr)[(*dc)++] = temp;
//            }
//            ptr = strchr(ptr, ',') ? strchr(ptr, ',') : strchr(ptr, '\0');
//        } else {
//            ptr++;
//        }
//    }
//}
//
///**
// * Parsing every piece of string from the received string line.
// * For each character in the string, we will generate assign a matching struct & address in memory,
// * while updating dc's value accordingly at the same time.
// * @param ptr A pointer to string line.
// * @param dc The counter of the data.
// * @param data_arr An array of data to which we would like to append stuff if relevant.
// */
//void handle_string(char *ptr, unsigned long *dc, word **data_arr, enum run_type rt) {
//    ptr = strchr(ptr, '"') + 1;
//    while (*ptr != '"' && *ptr) {
//        if (isalnum(*ptr)) {
//            if (rt == second_run_type) (*dc)++;
//            else {
//                word temp;
//                convert_to_10_bit_bin((size_t)*ptr, temp.value);
//                temp.address = (*dc);
//                *data_arr = (word *)realloc(*data_arr, sizeof(word) * ((*dc) + 1));
//                (*data_arr)[(*dc)++] = temp;
//            }
//        }
//        ptr++;
//    }
//
//    // Inserting an extra word to the data array that resembles the end of the string:
//    if (rt == second_run_type) {
//        (*dc)++;
//        return;
//    }
//
//    word temp;
//    convert_to_10_bit_bin(0, temp.value);
//    temp.address = (*dc);
//    *data_arr = (word *)realloc(*data_arr, sizeof(word) * ((*dc) + 1));
//    (*data_arr)[(*dc)++] = temp;
//}
//
//
//// Check for invalid usage of oprator (i.e
//void handle_operator(char *ptr, symbol **head, symbol **ent_table_head, unsigned long *pc, word **code_arr, issue ** errors_array, int *ec, int lc, enum run_type rt) {
//    if (!validate_operator_usage_in_str(ptr)) {
//        // ops_count is a single digit number, meaning it will have 1 char, therefore it will replace the "%d" and we
//        // don't need to allocate any extra memory (+1 for \0):
//        size_t errLen = strlen("Operator received wrong amount of args - Expected: %d.");
//        char *error = (char *)malloc((strlen("Operator received wrong amount of args - Expected: %d."))* sizeof(char));
//        snprintf(error, errLen, "Operator received wrong amount of args - Expected: %d.", (int)get_operator(ptr)->ops_count);
//        add_new_issue_to_arr(errors_array, ec, lc, error);
//        return;
//    }
//    // In any other case - encode the command and store it in the code array:
//    encode_cmd(ptr, head, ent_table_head, pc, code_arr, errors_array, ec, lc, rt);
//}
//
//
///**
// * Handle entry lines from the source code.
// * Basically detects and appends entry symbols to the symbols linked list.
// * @param head A pointer to the head of a linked list that consists of symbols.
// * @param ent_table_head A pointer to the head of a ll that consists of entry symbols.
// * @param ptr A pointer to a given line from the source code.
// * @param errors_array An array of errors which will be outputted to the user.
// * @param ec errors counter (index in errors array).
// * @param lc lines counter (lines index in source code file).
// */
//void handle_entry(symbol** head, symbol **ent_table_head, char *ptr, issue ** errors_array, int *ec, int lc) {
//    // Skip the extern word & define a token with the contents of the first symbol:
//    ptr = ptr + strlen(".entry") + 1;
//    char *symbolName = NULL, *token = strtok(ptr, ",");
//    // Appending each symbol to the symbols list:
//    while (token != NULL) {
//        symbolName = (char *)strdup(token);
//        append_unique(head, ent_table_head, symbolName, 0, symbol_entry, errors_array, ec, lc);
//        token = strtok(NULL, ",");
//    }
//}
//
///**
// * Handle extern lines from the source code.
// * Basically detects and appends extern symbols to the symbols linked list.
// * @param head A pointer to the head of a linked list that consists of symbols.
// * @param ptr A pointer to a given line from the source code.
// * @param errors_array An array of errors which will be outputted to the user.
// * @param ec errors counter (index in errors array).
// * @param lc lines counter (lines index in source code file).
// */
//void handle_extern(symbol** head, symbol **ent_table_head, char *ptr, issue ** errors_array, int *ec, int lc) {
//    // Skip the extern word & define a token with the contents of the first symbol:
//    ptr = ptr + strlen(".extern") + 1;
//    char *symbolName = NULL, *token = strtok(ptr, ",");
//    // Appending each symbol to the symbols list:
//    while (token != NULL) {
//        symbolName = (char *)strdup(token);
//        append_unique(head, ent_table_head, symbolName, 0, symbol_extern, errors_array, ec, lc);
//        token = strtok(NULL, ",");
//    }
//}
//
///**
// * A function in charge of determining the symbol type of the received string.
// * @param str A string which we would like to determine its type.
// * @return The type of the symbol (data / code / invalid).
// */
//enum symbol_type get_symbol_type(const char *str) {
//    char *ptr = (char *)str;
//    enum data_type curDataType = parse_str(ptr);
//    // Returning symbol_data if .data / .string / .struct in the string, or symbol_code if any op in string:
//    if (curDataType == sdata || curDataType == sstring || curDataType == sstruct) return symbol_data;
//    else if (curDataType == op) return symbol_code;
//    else if (curDataType == iextern) return symbol_extern;
//    else if (curDataType == ientry) return symbol_entry;
//    return symbol_invalid;
//}
//
//int handle_append_symbol_to_symbols_arr(symbol** head, symbol **ent_table_head, unsigned long extc, unsigned long entc, unsigned long pc, unsigned long dc, char **ptr, issue ** errors_array, int *ec, int lc, char *symbolName) {
//    enum symbol_type st = get_symbol_type(*ptr);
//    if (st == symbol_data)
//        return append_unique(head, ent_table_head, symbolName, dc, symbol_data, errors_array, ec, lc);
//    else if (st == symbol_code)
//        return append_unique(head, ent_table_head, symbolName, pc, symbol_code, errors_array, ec, lc);
//    else if (st == symbol_entry)
//        return append_unique(head, ent_table_head, symbolName, entc, symbol_entry, errors_array, ec, lc);
//    else if (st == symbol_extern)
//        return append_unique(head, ent_table_head, symbolName, extc, symbol_extern, errors_array, ec, lc);
//    return 0;
//}
//
///**
// * A function in charge of handling a case in which the received string contains a symbol_type.
// * @param head The head of the symbols linked list.
// * @param dc The counter of the data.
// * @param ptr A pointer to the current line.
// */
//int handle_symbol(symbol** head, symbol **ent_table_head, unsigned long extc, unsigned long entc, unsigned long pc, unsigned long dc, char **ptr, issue ** errors_array, int *ec, int lc) {
//    unsigned short symbolLen;
//    char *symbolName = NULL;
//    // Checking if the current line contains a symbol_type or not:
//
//    if (parse_str(*ptr) == symbol_dt) {
//        // Allocating memory to store the symbol_type & adding it to the symbols list:
//        symbolLen = (unsigned short)(strchr(*ptr, ':') - *ptr);
//        symbolName = (char *)strndup(*ptr, symbolLen);
//        *ptr += symbolLen + 1;
//        return handle_append_symbol_to_symbols_arr(head, ent_table_head, 0, 0, pc, dc, ptr, errors_array, ec, lc, symbolName);
//    }
//    return 1;
//}
//
//void handle_line_second_run(symbol **head, symbol **ent_table_head, unsigned long *mc, word **mem_arr, int lc, const char *line, issue **errors_array, int *ec) {
//    char *ptr = (char *)line;
//    enum run_type rt = second_run_type;
//    enum data_type curDataType = parse_str(ptr);
//    // Skipping any symbols (because we already declared them in the 1st run):
//    if (curDataType == symbol_dt)
//        ptr = strchr(ptr, ':') + 1;
//
//    // Skipping invalid lines:
//    curDataType = parse_str(ptr);
//    if (!validate_str(ptr, curDataType)) return;
//
//
//    // Only handling code "segment" (operations related fields that weren't initialized in the 1st run):
//    if (curDataType == op)
//        handle_operator(ptr, head, ent_table_head, mc, mem_arr, errors_array, ec, lc, rt);
//    else if (curDataType == sstring || curDataType == sdata || curDataType == sstruct || curDataType == iextern || curDataType == ientry)
//        return;
//}
//
//
///**
// * A function in charge of performing relevant allocations and calling the
// * matching functions based on the current line.
// * @param head The head of the symbols linked list.
// * @param dc The counter of the data.
// * @param line A string with the content of the current line.
// * @param data_arr An array of data to which we would like to append stuff if relevant.
// */
//void handle_line(symbol **head, symbol **ent_table_head, unsigned long *pc, word **code_arr, unsigned long *dc, const char *line, word **data_arr, issue **errors_array, int *ec, int lc) {
//    enum data_type curDataType;
//    char *ptr = (char *)line;
//    enum run_type rt = first_run_type;
//
//    // Handle symbols (if there are any):
//    if (!handle_symbol(head, ent_table_head, 0, 0, *pc, *dc, &ptr, errors_array, ec, lc)) return;
//
//    curDataType = parse_str(ptr);
//    if (!validate_str(ptr, curDataType)) {
//        add_new_issue_to_arr(errors_array, ec, lc, "Invalid instruction.");
//        return;
//    }
//
//    // Fetch the actual data type of the row (after "skipping" the symbol_type part):
//    if (curDataType == sstring)
//        handle_string(ptr, dc, data_arr, rt);
//    else if (curDataType == sdata)
//        handle_data(ptr, dc, data_arr, rt);
//    else if (curDataType == sstruct) {
//        // Handling data up to the first comma & string after the first comma:
//        handle_data(strtok(ptr, ","), dc, data_arr, rt);
//        handle_string(strtok(NULL, ","), dc, data_arr, rt);
//    } else if (curDataType == op)
//        handle_operator(ptr, head, ent_table_head, pc, code_arr, errors_array, ec, lc, rt);
//    else if (curDataType == iextern)
//        handle_extern(head, ent_table_head, ptr, errors_array, ec, lc);
//    else if (curDataType == ientry)
//        handle_entry(head, ent_table_head, ptr, errors_array, ec, lc);
//    else
//        add_new_issue_to_arr(errors_array, ec, lc, "Invalid instruction.");
//}
//
//
//void first_run(const char *content, symbol **head, symbol **ent_table_head, unsigned long *pc, word **code_arr, unsigned long *dc, word **data_arr, issue **errors_array, int *ec) {
//    char *ptr = strdup(content), *curLine = ptr, *nextLine = NULL;
//    int lc = 1; // Lines counter
//
//    while(curLine)
//    {
//        nextLine = strchr(curLine, '\n');
//        if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
//        curLine = trim(curLine); // Trim whitespaces.
//        // Skipping comments in assembler file:
//        if (*curLine != ';'){
//            if (!is_empty(curLine)) handle_line(head, ent_table_head, pc, code_arr, dc, curLine, data_arr, errors_array, ec, lc);
//        }
//        if (nextLine) *nextLine = '\n';  // then restore newline-char, just to be tidy
//        curLine = nextLine ? (nextLine+1) : NULL;
//        lc++;
//    }
//    free(ptr);
//}
//
//void second_run(char *content, symbol **head, symbol **ent_table_head, word **mem_arr, unsigned long *mc, issue **errors_array, int *ec) {
//    char * curLine = content, *nextLine = NULL;
//    int lc = 1; // Lines counter
//    while(curLine)
//    {
//        nextLine = strchr(curLine, '\n');
//        if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
//        curLine = trim(curLine); // Trim whitespaces.
//        // Skipping comments in assembler file:
//        if (*curLine != ';'){
//            if (!is_empty(curLine)) handle_line_second_run(head, ent_table_head, mc, mem_arr, lc, curLine, errors_array, ec);
//        }
//        if (nextLine) *nextLine = '\n';  // then restore newline-char, just to be tidy
//        curLine = nextLine ? (nextLine+1) : NULL;
//        lc++;
//    }
//}
//
//int validate_errors(issue *errors_arr, int ec) {
//    if (ec != 0) {
//        printf("\n\n\n~~~~~~~~~~~~~~~~ERRORS:~~~~~~~~~~~~~~~~\n");
//        print_issues(errors_arr, ec);
//        return 0;
//    }
//    return 1;
//}

//void assemble_files(char *fileName) {
//    printf("ENTRIES:\n");
//    write_file_custom_symbols_ll(str_cat_copy(fileName, ".ent"), get_entry_symbols_from_ll(symbols_table_head));
//    printf("\n\nEXTERNS:\n");
//    write_file_custom_symbols_ll(str_cat_copy(fileName, ".ext"), get_extern_symbols_from_ll(symbols_table_head));
//    printf("\n\nOBJECT:\n");
//    write_file_custom_word_arr("EREZKING.ob", mem_arr, 100, pc + dc);
//}

void assemble_machine_code(char *fileName, char *content) {
    issue *errors_array = NULL;
    unsigned long pc = 100, dc = 0, mc = 100;
    symbol *symbols_table_head = NULL, *ptr = NULL, *ent_table_head = NULL, *ext_table_head = NULL;
    word *data_arr = NULL, *code_arr = NULL, *mem_arr = NULL;
    int ec = 0; // Errors counter

    // Perform first run & update addresses based on instructions counter & data counter:
    first_run(content, &symbols_table_head, &ent_table_head, &pc, &code_arr, &dc, &data_arr, &errors_array, &ec);
    mem_arr = append_word_arr(&code_arr, pc, &data_arr, dc);
    update_list_addresses(symbols_table_head, pc, symbol_data);
    update_list_addresses(ent_table_head, pc, symbol_data);

    // Perform 2nd run & update the symbols' table head:
    second_run(content, &symbols_table_head, &ent_table_head, &mem_arr, &mc, &errors_array, &ec);

    // Exiting if we found errors:
    if (!validate_errors(errors_array, ec)) return;

    ptr = symbols_table_head;
    symbols_table_head = generate_symbols_table(symbols_table_head, ent_table_head);

    // Exiting if we found errors:
    if (!validate_errors(errors_array, ec)) return;

    // Create matching .ent & .ext files:
    printf("ENTRIES:\n");
    write_file_custom_symbols_ll(str_cat_copy(fileName, ".ent"), get_entry_symbols_from_ll(symbols_table_head));
    printf("\n\nEXTERNS:\n");
    write_file_custom_symbols_ll(str_cat_copy(fileName, ".ext"), get_extern_symbols_from_ll(symbols_table_head));
    printf("\n\nOBJECT:\n");
    write_file_custom_word_arr("EREZKING.ob", mem_arr, 100, pc + dc);


    free_list(ptr);
    free_list(symbols_table_head);
    free_list(ent_table_head);
    free_list(ext_table_head);
}