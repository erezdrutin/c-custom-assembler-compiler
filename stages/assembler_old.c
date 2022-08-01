//#include "assembler_old.h"
//#include "../ds/symbol_type.h"
//#include "../utils/files.h"
//#include "../utils/strings.h"
//#include <string.h>
//#include <stdio.h>
//#include <ctype.h>
//
//const char *ops[16] = { "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
//                        "dec", "jmp", "bne", "get", "prn", "jsr", "rts", "hlt"};
//
///**
// * An enum to determine what to do at a given moment whilst parsing the ".am" file's content.
// * Data types: symbol_type (label), sx (store x), ix (instruction x), op (operation), invalid (none of the other datatypes).
// */
//enum data_type { symbol_type, sdata, sstring, sstruct, iextern, ientry, op, invalid };
//
//void print_data_arr(data * arr, size_t dc) {
//    size_t i;
//    for (i = 0; i < dc; i++) {
//        printf("value: %s, address: %u\n", arr[i].value, arr[i].address);
//    }
//}
//
//int get_op_code (char* str) {
//    int len = (size_t) sizeof(str) / sizeof(str[0]), i;
//    for (i = 0; i < len; ++i) {
//        // If both strings are equal - return matching op_code:
//        if (!strcmp(ops[i], str)) {
//            return i;
//        }
//    }
//    return -1;
//}
//
//enum data_type parse_str(char* str) {
//    if (strchr(str, ':')) return symbol_type;
//    if (strstr(str, ".data")) return sdata;
//    if (strstr(str, ".string")) return sstring;
//    if (strstr(str, ".struct")) return sstruct;
//    if (strstr(str, ".extern")) return iextern;
//    if (strstr(str, ".entry")) return ientry;
//    if (get_op_code(str) != -1) return op;
//    return invalid;
//}
//
//void printDigit(long long N)
//{
//    long long r;
//
//    // Base Case
//    if (N == 0) {
//        return;
//    }
//
//    // Extract the last digit
//    r = N % 10;
//
//    // Recursive call to next
//    // iteration
//    printDigit(N / 10);
//
//    // Print r
//    printf("%lld ", r);
//}
//
////char * ll_to_string(long long num) {
////    int length = snprintf(NULL, 0, "%lld", num);
////    char* str = malloc(length + 1);
////    snprintf(str, length + 1, "%lld", num);
////    return str;
////}
//
///**
// * A function in charge of converting a decimal number (long long) to its matching binary 10 bit representation.
// * @param number The number that we would like to convert to 10 bit binary representation.
// * @param value The char array in which we should store the 10 bit binary representation.
// */
//void convert_to_10_bit_bin(size_t number, char value[11])
//{
//    int i;
//    for (i = 9; i >= 0; i--) {
//        // right shift & check the state of the first bit:
//        value[9 - i] = (number >> i) & 1 ? '1' : '0';
//    }
//    value[10] = '\0';
//}
//
//
///**
// * Parsing every piece of data from the received data line.
// * For each data, we will generate assign a matching data struct & address in memory, while updating dc's value
// * accordingly at the same time.
// * @param ptr A pointer to a data line.
// * @param dc The data counter.
// * @param data_arr An array of data structs.
// * @return A pointer to the data structs array after updating it.
// */
//data * handle_data(char *ptr, unsigned long *dc, data **data_arr) {
//    while (*ptr) { // While there are more characters to process...
//        if ( isdigit(*ptr) || ( (*ptr=='-'||*ptr=='+') && isdigit(*(ptr+1)) )) {
//            data temp;
//            convert_to_10_bit_bin(strtoll(ptr, &ptr, 10), temp.value);
//            temp.address = (*dc);
//            *data_arr = (data *)realloc(*data_arr, (*dc) + 1);
//            (*data_arr)[(*dc)++] = temp;
//            ptr = strchr(ptr, ',') ? strchr(ptr, ',') : strchr(ptr, '\0');
//        } else {
//            ptr++;
//        }
//    }
//    return *data_arr;
//}
//
//data * handle_string(char *ptr, unsigned long *dc, data **data_arr) {
//    ptr = strchr(ptr, '"') + 1;
//    while (*ptr != '"' && *ptr) {
//        if (isalnum(*ptr)) {
//            data temp;
//            convert_to_10_bit_bin((size_t)*ptr, temp.value);
//            temp.address = (*dc);
//            *data_arr = (data *)realloc(*data_arr, (*dc) + 1);
//            (*data_arr)[(*dc)++] = temp;
//        }
//        ptr++;
//    }
//    return *data_arr;
//}
//
//char * handle_symbol(symbol** head, unsigned long dc, enum data_type dt, char **ptr) {
//    if (dt == symbol_type) {
//        unsigned short symbolLen = (unsigned short)(strchr(*ptr, ':') - *ptr);
//        char *symbolName = (char *)strndup(*ptr, symbolLen);
//        append(head, symbolName, dc, symbol_data);
//        *ptr += symbolLen + 1;
//        free(symbolName);
//    }
//    return *ptr;
//}
//
//data * handle_statements(symbol** head, unsigned long* dc, enum data_type dt, const char* str, data * data_arr) {
//    char *ptr = (char *)str;
//
//    // Handling symbols:
//    dt = parse_str(handle_symbol(head, *dc, dt, &ptr));
//
//    // Determining the type of the rest of the line and acting accordingly:
//    if (dt == sstring) {
//        return handle_string(ptr, dc, &data_arr);
//    } else if (dt == sstruct) {
//        data_arr = handle_data(strtok(ptr, ","), dc, &data_arr);
//        return handle_string(strtok(NULL, ","), dc, &data_arr);
//    } else if (dt == sdata) {
//        return handle_data(ptr, dc, &data_arr);
//    }
//    return data_arr;
//}
//
//symbol* generate_symbols(char* content) {
//    data * data_arr = NULL;
//    unsigned long ic = 0, dc = 0;
//    symbol* symbols_table_head = NULL;
//
//    short symbol_flag = 0;
//    char * curLine = content;
//    while(curLine)
//    {
//        char * nextLine = strchr(curLine, '\n');
//        char * firstWord;
//        if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
//        // Skipping comments in assembler file:
//        if (!strstr(curLine, "//")){
//            // do stuff:
//            curLine = trim(curLine); // Trim whitespaces.
//            enum data_type cur_dt = parse_str(curLine); // Find current row's first "word" data type.
//
//            // Turn symbol_type flag:
//            if (cur_dt == symbol_type || cur_dt == sdata || cur_dt == sstring || cur_dt == sstruct) {
//                data_arr = handle_statements(&symbols_table_head, &dc, cur_dt, curLine, data_arr);
////                printf("\n\n\n%s\n------------------------------------------------\n", curLine);
////                print_data_arr(data_arr, dc);
////                printf("\n\n\n");
//            }
//        }
//        if (nextLine) *nextLine = '\n';  // then restore newline-char, just to be tidy
//        curLine = nextLine ? (nextLine+1) : NULL;
//    }
//    // Initializing first element in data_arr with address 0:
////    if (data_arr != NULL) data_arr->address = 0;
//    print_data_arr(data_arr, dc);
////    printf("DATATATA: %s\n\n\n", data_arr);
//    return symbols_table_head;
//}
//
//void create_symbols_table(char* content) {
//    size_t ic = 0, dc = 0;
//    const char* ptr = content;
//    char *temp = NULL, *macroDefinition = NULL, *macroContent = NULL, *macroName = NULL;
//
////    char longString[] = "This is a long string.\nIt has multiple lines of text in it.\nWe want to examine each of these lines separately.\nSo we will do that.";
//    char * curLine = content;
//    while(curLine)
//    {
//        char * nextLine = strchr(curLine, '\n');
//        if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
//        enum data_type cur_dt = parse_str(curLine);
//        printf("curLine=[%s], curDataType=[%d]\n", curLine, cur_dt);
//        if (nextLine) *nextLine = '\n';  // then restore newline-char, just to be tidy
//        curLine = nextLine ? (nextLine+1) : NULL;
//    }
//
////    // Searching for macros in the content string:
////    while ((temp = strchr(ptr, ':'))) {
////        // Extracting the name, content & definition parts of the current macro from the content string:
////        macroName = extract_macro_name(temp);
////        macroContent = extract_macro_content(temp);
////        macroDefinition = extract_macro_definition(temp);
////
////        // Removing the macro definition:
////        str_replace(content, macroDefinition, "");
////        // Replacing each occurrence of the macro with its content:
////        str_replace_macro(content, macroName, macroContent);
////
////        // Freeing the memory used in the process:
////        free(macroName);
////        free(macroContent);
////        free(macroDefinition);
////    }
//}
//
//void first_parse(char* content){
//    size_t ic = 0;
//
//}
//
void second_parse(){

}
//
//void assembler(char* fileName) {
//    char* content = read_file(fileName);
//    symbol t1;
//}