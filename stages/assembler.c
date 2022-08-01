#include "assembler.h"
#include "../ds/data.h"
#include "../ds/operators.h"
#include "../utils/conversions.h"
#include "../utils/strings.h"
#include <string.h>
#include <ctype.h>
#include <printf.h>

enum data_type { symbol_dt, sdata, sstring, sstruct, iextern, ientry, op, invalid };
enum are_encoding { are_absolute, are_external, are_relocatable};

//const char *ops[16] = { "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
//                        "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};

//int get_op_code (char* str) {
//    int len = (size_t) sizeof(str) / sizeof(str[0]), i;
//    for (i = 0; i < len; ++i) {
//        if (strstr(str, ops[i])) {
//            return i;
//        }
//    }
//    return -1;
//}

char * parse_are(enum are_encoding are) {
    return are == are_absolute ? "00" : are == are_external ? "01" : "10";
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
 * Parsing every piece of data from the received data line.
 * For each data, we will generate assign a matching data struct & address in memory, while updating dc's value
 * accordingly at the same time.
 * @param ptr A pointer to a data line.
 * @param dc The data counter.
 * @param data_arr An array of data structs.
 */
void handle_data(char *ptr, unsigned long *dc, data **data_arr) {
    while (*ptr) { // While there are more characters to process...
        if ( isdigit(*ptr) || ( (*ptr=='-'||*ptr=='+') && isdigit(*(ptr+1)) )) {
            data temp;
            convert_to_10_bit_bin(strtoll(ptr, &ptr, 10), temp.value);
            temp.address = (*dc);
            *data_arr = (data *)realloc(*data_arr, sizeof(data)*((*dc) + 1));
            (*data_arr)[(*dc)++] = temp;
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
void handle_string(char *ptr, unsigned long *dc, data **data_arr) {
    ptr = strchr(ptr, '"') + 1;
    while (*ptr != '"' && *ptr) {
        if (isalnum(*ptr)) {
            data temp;
            convert_to_10_bit_bin((size_t)*ptr, temp.value);
            temp.address = (*dc);
            *data_arr = (data *)realloc(*data_arr, sizeof(data)*((*dc) + 1));
            (*data_arr)[(*dc)++] = temp;
        }
        ptr++;
    }
}

enum are_encoding determine_are(char *ptr) {
    return are_absolute;
}

char *extract_src_address(char *ptr) {
    return ptr;
}

char *extract_dest_address(char *ptr) {
    return ptr;
}


// Check for invalid usage of oprator (i.e
void handle_operator(char *ptr, unsigned long *dc, data **data_arr, issue ** errors_array, int *ec, int lc) {
    if (!validate_operator_usage(ptr)) {
        // ops_count is a single digit number, meaning it will have 1 char, therefore it will replace the "%d" and we
        // don't need to allocate any extra memory:
        size_t errLen = strlen("Operator received wrong amount of args - Expected: %d.");
        char *error = (char *)malloc((strlen("Operator received wrong amount of args - Expected: %d."))* sizeof(char));
        snprintf(error, errLen, "Operator received wrong amount of args - Expected: %d.", (int)get_operator(ptr)->ops_count);
        add_new_issue_to_arr(errors_array, ec, lc, error);

        // Adding an error message:
//        char * error = (char *)malloc(57*sizeof(char));
//        snprintf(error, 57, "Operator received wrong amount of args, Expected: %d.",
//                 (int)get_operator(ptr)->ops_count);
//        add_new_issue_to_arr(errors_array, ec, lc, error);
        return;
    }
    char *are_encoding = parse_are(determine_are(ptr)); // 0-1
    char *src_address = extract_src_address(ptr); // 2-3
    char *dest_address = extract_dest_address(ptr); // 4-5
    int opcode = get_operator_index(ptr); // 6-9

//    ptr = strchr(ptr, '"') + 1;
//    while (*ptr != '"' && *ptr) {
//        if (isalnum(*ptr)) {
//            data temp;
//            convert_to_10_bit_bin((size_t)*ptr, temp.value);
//            temp.address = (*dc);
//            *data_arr = (data *)realloc(*data_arr, sizeof(data)*((*dc) + 1));
//            (*data_arr)[(*dc)++] = temp;
//        }
//        ptr++;
//    }
}


/**
 * Handle entry lines from the source code.
 * Basically detects and appends entry symbols to the symbols linked list.
 * @param head A pointer to the head of a linked list that consists of symbols.
 * @param ptr A pointer to a given line from the source code.
 * @param errors_array An array of errors which will be outputted to the user.
 * @param ec errors counter (index in errors array).
 * @param lc lines counter (lines index in source code file).
 */
void handle_entry(symbol** head, char *ptr, issue ** errors_array, int *ec, int lc) {
    // Skip the extern word & define a token with the contents of the first symbol:
    ptr = ptr + strlen(".entry") + 1;
    char *symbolName = NULL, *token = strtok(ptr, ",");
    // Appending each symbol to the symbols list:
    while (token != NULL) {
        symbolName = (char *)strdup(token);
        append_unique(head, symbolName, 0, symbol_entry, errors_array, ec, lc);
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
void handle_extern(symbol** head, char *ptr, issue ** errors_array, int *ec, int lc) {
    // Skip the extern word & define a token with the contents of the first symbol:
    ptr = ptr + strlen(".extern") + 1;
    char *symbolName = NULL, *token = strtok(ptr, ",");
    // Appending each symbol to the symbols list:
    while (token != NULL) {
        symbolName = (char *)strdup(token);
        append_unique(head, symbolName, 0, symbol_extern, errors_array, ec, lc);
        token = strtok(NULL, ",");
    }
}

/**
 * A function in charge of determining the symbol type of the received string.
 * @param str A string which we would like to determine its type.
 * @return The type of the symbol (data / code / invalid).
 */
enum symbol_type get_symbol_type(const char *str) {
    char *ptr = (char *)str;
    enum data_type curDataType = parse_str(ptr);
    // Returning symbol_data if .data / .string / .struct in the string, or symbol_code if any op in string:
    return (curDataType == sdata || curDataType == sstring || curDataType == sstruct) ? symbol_data : curDataType == op ? symbol_code : symbol_invalid;
}

/**
 * A function in charge of handling a case in which the received string contains a symbol_type.
 * @param head The head of the symbols linked list.
 * @param dc The counter of the data.
 * @param ptr A pointer to the current line.
 */
int handle_symbol(symbol** head, unsigned long dc, char **ptr, issue ** errors_array, int *ec, int lc) {
    unsigned short symbolLen;
    char *symbolName = NULL;
    enum symbol_type curSymbolType = get_symbol_type(*ptr);
    // Checking if the current line contains a symbol_type or not:
    if (parse_str(*ptr) == symbol_dt) {
        // Allocating memory to store the symbol_type & adding it to the symbols list:
        symbolLen = (unsigned short)(strchr(*ptr, ':') - *ptr);
        symbolName = (char *)strndup(*ptr, symbolLen);
        *ptr += symbolLen + 1;
        return append_unique(head, symbolName, dc, get_symbol_type(*ptr), errors_array, ec, lc);
    }
    return 1;
}


/**
 * A function in charge of performing relevant allocations and calling the
 * matching functions based on the current line.
 * @param head The head of the symbols linked list.
 * @param dc The counter of the data.
 * @param line A string with the content of the current line.
 * @param data_arr An array of data to which we would like to append stuff if relevant.
 */
void handle_line(symbol **head, unsigned long *dc, const char *line, data **data_arr, issue **errors_array, int *ec, int lc) {
    enum data_type curDataType;
    char *ptr = (char *)line;

    // Handle symbols (if there are any):
    if (!handle_symbol(head, *dc, &ptr, errors_array, ec, lc)) return;

    // Fetch the actual data type of the row (after "skipping" the symbol_type part):
    curDataType = parse_str(ptr);
    if (curDataType == sstring)
        handle_string(ptr, dc, data_arr);
    else if (curDataType == sdata)
        handle_data(ptr, dc, data_arr);
    else if (curDataType == sstruct) {
        // Handling data up to the first comma & string after the first comma:
        handle_data(strtok(ptr, ","), dc, data_arr);
        handle_string(strtok(NULL, ","), dc, data_arr);
    } else if (curDataType == op)
        handle_operator(ptr, dc, data_arr, errors_array, ec, lc);
    else if (curDataType == iextern)
        handle_extern(head, ptr, errors_array, ec, lc);
    else if (curDataType == ientry)
        handle_entry(head, ptr, errors_array, ec, lc);
    else
        add_new_issue_to_arr(errors_array, ec, lc, "Invalid instruction.");
}

symbol* generate_symbols(char* content) {
    issue *errors_array = NULL;
    unsigned long ic = 0, dc = 0;
    symbol* symbols_table_head = NULL;
    data *data_arr = NULL, *code_arr = NULL;
    char * curLine = content, *nextLine = NULL;
    int lc = 1, ec = 0; // Lines counter

    while(curLine)
    {
        nextLine = strchr(curLine, '\n');
        if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
        // Skipping comments in assembler file:
        if (!strstr(curLine, "//")){
            curLine = trim(curLine); // Trim whitespaces.
            if (!is_empty(curLine)) handle_line(&symbols_table_head, &dc, curLine, &data_arr, &errors_array, &ec, lc);
        }
        if (nextLine) *nextLine = '\n';  // then restore newline-char, just to be tidy
        curLine = nextLine ? (nextLine+1) : NULL;
        lc++;
    }
    print_data_arr(data_arr, dc);
    print_issues(errors_array, ec);
    return symbols_table_head;
}


//            handle_symbol(&symbols_table_head, dc, parse_str(curLine), &curLine);
//            cur_dt = parse_str(curLine);
//            if (cur_dt == sstring) {
//                handle_string(curLine, &dc, &data_arr);
//            } else if (cur_dt == sdata) {
//                handle_data(curLine, &dc, &data_arr);
//            } else if (cur_dt == sstruct) {
//                handle_data(strtok(curLine, ","), &dc, &data_arr);
//                handle_string(strtok(NULL, ","), &dc, &data_arr);
//            }