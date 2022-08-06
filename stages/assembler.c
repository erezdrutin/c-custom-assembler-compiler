#include "assembler.h"
#include "../ds/word.h"
#include "../ds/operators.h"
#include "../utils/conversions.h"
#include "../utils/string_utils.h"
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

/**
 * A function in charge of determining the symbol type of the received string.
 * @param str A string which we would like to determine its type.
 * @return The type of the symbol (data / code / invalid).
 */
enum symbol_type get_symbol_type(const char *str) {
    char *ptr = (char *)str;
    enum data_type curDataType = parse_str(ptr);
    // Returning symbol_data if .data / .string / .struct in the string, or symbol_code if any op in string:
    if (curDataType == sdata || curDataType == sstring || curDataType == sstruct) return symbol_data;
    else if (curDataType == op) return symbol_code;
    else if (curDataType == iextern) return symbol_extern;
    else if (curDataType == ientry) return symbol_entry;
    return symbol_invalid;
}

int handle_append_symbol_to_symbols_arr(symbol** head, symbol **ent_table_head, unsigned long extc, unsigned long entc, unsigned long pc, unsigned long dc, char **ptr, issue ** errors_array, int *ec, int lc, char *symbolName) {
    enum symbol_type st = get_symbol_type(*ptr);
    if (st == symbol_data)
        return append_unique(head, ent_table_head, symbolName, dc, symbol_data, errors_array, ec, lc);
    else if (st == symbol_code)
        return append_unique(head, ent_table_head, symbolName, pc, symbol_code, errors_array, ec, lc);
    else if (st == symbol_entry)
        return append_unique(head, ent_table_head, symbolName, entc, symbol_entry, errors_array, ec, lc);
    else if (st == symbol_extern)
        return append_unique(head, ent_table_head, symbolName, extc, symbol_extern, errors_array, ec, lc);
    return 0;
}

/**
 * A function in charge of handling a case in which the received string contains a symbol_type.
 * @param head The head of the symbols linked list.
 * @param dc The counter of the data.
 * @param ptr A pointer to the current line.
 */
int handle_symbol(symbol** head, symbol **ent_table_head, unsigned long extc, unsigned long entc, unsigned long pc, unsigned long dc, char **ptr, issue ** errors_array, int *ec, int lc) {
    unsigned short symbolLen;
    char *symbolName = NULL;
    // Checking if the current line contains a symbol_type or not:

    if (parse_str(*ptr) == symbol_dt) {
        // Allocating memory to store the symbol_type & adding it to the symbols list:
        symbolLen = (unsigned short)(strchr(*ptr, ':') - *ptr);
        symbolName = (char *)strndup(*ptr, symbolLen);
        *ptr += symbolLen + 1;
        return handle_append_symbol_to_symbols_arr(head, ent_table_head, 0, 0, pc, dc, ptr, errors_array, ec, lc, symbolName);
    }
    return 1;
}

void handle_line_second_run(symbol **head, symbol **ent_table_head, unsigned long *mc, word **mem_arr, int lc, const char *line, issue **errors_array, int *ec) {
    char *ptr = (char *)line;
    enum run_type rt = second_run_type;
    enum data_type curDataType = parse_str(ptr);
    // Skipping any symbols (because we already declared them in the 1st run):
    if (curDataType == symbol_dt)
        ptr = strchr(ptr, ':') + 1;


    // Only handling code "segment" (operations related fields that weren't initialized in the 1st run):
    curDataType = parse_str(ptr);
    if (curDataType == op)
        handle_operator(ptr, head, ent_table_head, mc, mem_arr, errors_array, ec, lc, rt);
    else if (curDataType == sstring || curDataType == sdata || curDataType == sstruct || curDataType == iextern || curDataType == ientry)
        return;
    else
        add_new_issue_to_arr(errors_array, ec, lc, "Invalid instruction.");
}


/**
 * A function in charge of performing relevant allocations and calling the
 * matching functions based on the current line.
 * @param head The head of the symbols linked list.
 * @param dc The counter of the data.
 * @param line A string with the content of the current line.
 * @param data_arr An array of data to which we would like to append stuff if relevant.
 */
void handle_line(symbol **head, symbol **ent_table_head, unsigned long *pc, word **code_arr, unsigned long *dc, const char *line, word **data_arr, issue **errors_array, int *ec, int lc, enum run_type rt) {
    enum data_type curDataType;
    char *ptr = (char *)line;

    // Handle symbols (if there are any):
    // At the end of the run we should replace the addresses stored in this method based on the symbol type:
    if (!handle_symbol(head, ent_table_head, 0, 0, *pc, *dc, &ptr, errors_array, ec, lc)) return;

    // Fetch the actual data type of the row (after "skipping" the symbol_type part):
    curDataType = parse_str(ptr);
    if (curDataType == sstring)
        handle_string(ptr, dc, data_arr, rt);
    else if (curDataType == sdata)
        handle_data(ptr, dc, data_arr, rt);
    else if (curDataType == sstruct) {
        // Handling data up to the first comma & string after the first comma:
        handle_data(strtok(ptr, ","), dc, data_arr, rt);
        handle_string(strtok(NULL, ","), dc, data_arr, rt);
    } else if (curDataType == op)
        handle_operator(ptr, head, ent_table_head, pc, code_arr, errors_array, ec, lc, rt);
    else if (curDataType == iextern)
        handle_extern(head, ent_table_head, ptr, errors_array, ec, lc);
    else if (curDataType == ientry)
        handle_entry(head, ent_table_head, ptr, errors_array, ec, lc);
    else
        add_new_issue_to_arr(errors_array, ec, lc, "Invalid instruction.");
}

symbol* generate_symbols(char* content) {
    issue *errors_array = NULL;
    unsigned long pc = 0, dc = 0;
    symbol *symbols_table_head = NULL, *ent_table_head = NULL;
    word *data_arr = NULL, *code_arr = NULL, *mem_arr = NULL;
    char * curLine = content, *nextLine = NULL;
    int lc = 1, ec = 0; // Lines counter

    while(curLine)
    {
        nextLine = strchr(curLine, '\n');
        if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
        // Skipping comments in assembler file:
        if (!strstr(curLine, "//")){
            curLine = trim(curLine); // Trim whitespaces.
            if (!is_empty(curLine)) handle_line(&symbols_table_head, &ent_table_head, &pc, &code_arr, &dc, curLine, &data_arr, &errors_array, &ec, lc, first_run_type);
        }
        if (nextLine) *nextLine = '\n';  // then restore newline-char, just to be tidy
        curLine = nextLine ? (nextLine+1) : NULL;
        lc++;
    }
    printf("\n\n\n~~~~~~~~~~~~~~~~MERGED:~~~~~~~~~~~~~~~~\n");
    print_data_arr(append_word_arr(&code_arr, pc, &data_arr, dc), pc + dc);

    printf("\n\n\n~~~~~~~~~~~~~~~~ERRORS:~~~~~~~~~~~~~~~~\n");
    print_issues(errors_array, ec);
    printf("\n\n\n~~~~~~~~~~~~~~~~SYMBOLS:~~~~~~~~~~~~~~~~\n");
    return symbols_table_head;
}


void first_run(const char *content, symbol **head, symbol **ent_table_head, unsigned long *pc, word **code_arr, unsigned long *dc, word **data_arr, issue **errors_array, int *ec) {
    char *ptr = strdup(content), *curLine = ptr, *nextLine = NULL;
    int lc = 1; // Lines counter

    while(curLine)
    {
        nextLine = strchr(curLine, '\n');
        if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
        curLine = trim(curLine); // Trim whitespaces.
        // Skipping comments in assembler file:
        if (*curLine != ';'){
            if (!is_empty(curLine)) handle_line(head, ent_table_head, pc, code_arr, dc, curLine, data_arr, errors_array, ec, lc, first_run_type);
        }
        if (nextLine) *nextLine = '\n';  // then restore newline-char, just to be tidy
        curLine = nextLine ? (nextLine+1) : NULL;
        lc++;
    }
    free(ptr);
}

void second_run(char *content, symbol **head, symbol **ent_table_head, word **mem_arr, unsigned long *mc, issue **errors_array, int *ec) {
    char * curLine = content, *nextLine = NULL;
    int lc = 1; // Lines counter
    while(curLine)
    {
        nextLine = strchr(curLine, '\n');
        if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
        curLine = trim(curLine); // Trim whitespaces.
        // Skipping comments in assembler file:
        if (*curLine != ';'){
            if (!is_empty(curLine)) handle_line_second_run(head, ent_table_head, mc, mem_arr, lc, curLine, errors_array, ec);
        }
        if (nextLine) *nextLine = '\n';  // then restore newline-char, just to be tidy
        curLine = nextLine ? (nextLine+1) : NULL;
        lc++;
    }
}

int validate_errors(issue *errors_arr, int ec) {
    if (ec != 0) {
        printf("\n\n\n~~~~~~~~~~~~~~~~ERRORS:~~~~~~~~~~~~~~~~\n");
        print_issues(errors_arr, ec);
        return 0;
    }
    return 1;
}

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
    ptr = symbols_table_head;
    symbols_table_head = generate_symbols_table(symbols_table_head, ent_table_head);

    // Exiting if we found errors:
    if (!validate_errors(errors_array, ec)) return;

    // Fetching external symbols from the symbols table:
//    ext_table_head = get_extern_symbols_from_ll(symbols_table_head);

    // Create matching .ent & .ext files:
    printf("ENTRIES:\n");
    write_file_custom_symbols_ll(str_cat_copy(fileName, ".ent"), get_entry_symbols_from_ll(symbols_table_head));
    printf("\n\nEXTERNS:\n");
    write_file_custom_symbols_ll(str_cat_copy(fileName, ".ext"), get_extern_symbols_from_ll(symbols_table_head));
    printf("\n\nOBJECT:\n");
    write_file_custom_word_arr("EREZKING.ob", mem_arr, 100, pc + dc);



//    printf("\n\nSIU:\n\n");
//    printf("\n\nENTRIES:\n");
//    printList(get_entry_symbols_from_ll(symbols_table_head));
//    write_file_custom_symbols_ll("AYOOOO", get_entry_symbols_from_ll(symbols_table_head));
//    printf("\n\nEXTERNALS:\n");
//    printList(get_extern_symbols_from_ll(symbols_table_head));
//    write_file_custom_symbols_ll("AYOOOO2", get_extern_symbols_from_ll(symbols_table_head));
//    free_list(ptr);
//    free_list(symbols_table_head);
//    write_file_custom_symbols_ll("AYOO", symbols_table_head, symbol_entry);
//    write_file_custom_symbols_ll("AYOO2", symbols_table_head, symbol_extern);



//    write_file_custom_symbols_ll("BRUH", symbols_table_head, symbol_extern);

//    pc = 0, dc = 0;
//    first_run(content, &symbols_table_head, &pc, &code_arr, &dc, &data_arr, &errors_array, &ec);
//    mem_arr = append_word_arr(&code_arr, pc, &data_arr, dc);
//    printf("SIUUU\n\n%s\n\n", content);
//    printf("\n\n\n~~~~~~~~~~~~~~~~MERGED:~~~~~~~~~~~~~~~~\n");
//    print_data_arr(mem_arr, pc + dc);
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