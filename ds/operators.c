#include "operators.h"
#include "../utils/string_utils.h"
#include "../utils/conversions.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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

void handle_symbol_in_op(operator *op, char *str, symbol ** head, word **code_arr) {
    // Do something with structs and append relevant encoding to code_arr.
}

void handle_struct_in_op(operator *op, char *str, symbol ** head, word **code_arr) {
    // Do something with structs and append relevant encoding to code_arr.
}

void handle_register_in_op(operator *op, char *reg, symbol ** head, word **code_arr) {
    // Do something with structs and append relevant encoding to code_arr.
}

/**
 * A function in charge of validating that a label is valid.
 * A valid label starts with an alphabet letter which is either small or capital, followed by a sequence of characters
 * that are alphanumeric. In addition, a valid label may consist of up to 30 characters.
 * @param str A string to check if it matches the definition of a valid label.
 * @return True (1) / False (0).
 */
int is_label(char *str) {
    // Comes before a command / .data / .string / .struct
    // Starts with alphabet (small/CAP), followed by a sequence of alphabet (small/CAP) letters / digits,
    // and is up to 30 chars.
    int i;
    // Validate that the label starts with an alphabet letter:
    if (!((*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z')) || strlen(str) > 30) return 0;
    // Validating that every other letter is alphanumeric:
    for (i = 1; i <= strlen(str); i++) {
        if (!((str[i] >= '0' && str[i] <= '9') || (*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z'))) {
            return 0;
        }
    }
    return 1;
}

/**
 * A function in charge of checking whether the received string is a register or not.
 * @param str A string representing an operand (probably).
 * @return True (1) if the string is a register or False (0) if not.
 */
int is_register(char *str) {
    int i;
    // Initializing with '\0' at the end (therefore we need 3 chars = strlen("r%d")):
    char *reg = (char *)malloc(strlen("r%d"));
    for (i = 0; i < 8; i++){
        snprintf(reg, strlen("r%d"), "r%d", i);
        if (!strcmp(reg, str)) {
            free(reg);
            return 1;
        }
    }
    free(reg);
    return 0;
}

int is_struct_access_call(const char *str) {
    int flag = 1;
    char *ptr = strdup(str), *st, *ind;
    char * token;

    if (count_char_in_string(str, '.') != 1) {
        free(ptr);
        return 0;
    }

    token = strtok(ptr, ".");
    st = strdup(trim(token));
    token = strtok(NULL, ".");
    ind = strdup(trim(token));

    if (count_char_in_string(str, '.') != 1 || (*ind != '1' && *ind != '2') || !is_label(st)) {
        flag = 0;
    }

    free(ptr);
    free(st);
    free(ind);
    return flag;
}

// Operations encoding:
// XX = src addressing method (0-3)
// YY = dest addressing method (0-3)
// ZZ = A, R, E bits
// mov (0) operation word:
// 0000-XX-YY-ZZ
// jmp (9) operation word:
// 1001-XX-YY-ZZ

// Examples:
// mov #-1,r2
// 0000-00-11-


// A, R, E (ZZ):
// 00 --> Defined directly from the src file, without usage of other variables or whatever.
// 01 --> Defined by accessing external variable(s), only for "words" that are addresses.
// 10 --> Defined by accessing a symbol that either exists already or will exist somewhere later in the file.

// If src's size == dest size --> 00
// If src is external --> external --> 01
// If src's size < dest size --> relocatable --> 10


// *str --> op, src, dest --> XopXopXopXop - YsrcYsrc - ZdestZdest - MareMare
// src --> AAAAAAAA-MareMare (10 bits, 8 to represent address + 2 for ARE)
// dest --> BBBBBBBB-MareMare (10 bits, 8 to represent address + 2 for ARE)
// If either src / dest is a struct --> Another word with struct index {00000001-00} OR {00000010-00}

int isNumber(char *s)
{
    int i;
    for (i = 0; s[i]!= '\0'; i++)
    {
        if (!(isdigit(s[i]) || ((s[i] == '-' || s[i]=='+') && isdigit(s[i+1]))))
            return 0;
    }
    return 1;
}

int is_str_in_arr(char **arr, char *str) {
    int i;
    size_t len = sizeof(arr)/sizeof(arr[0]);
    for(i = 0; i < len; ++i)
    {
        if(!strcmp(arr[i], str))
        {
            return 1;
        }
    }
    return 0;
}

int is_str_in_blacklist(char *str) {
    int i, limit = ((REGISTERS_NUM) >= (OPS_NUM)) ? (REGISTERS_NUM) : (OPS_NUM);

//    for (i = 0; i < limit; i++) {
//        if (i < REGISTERS_NUM && !strcmp(ops[i].name))
//        if (!strcmp(ops[i].name, str)) return 1;
//    }
//    for (i = 0; i < 8; i)
    return 0;
}

/**
 * A function in charge of determining the appropriate addressing method for the received string (operand).
 * @param str A string that represents an operand.
 * @return A value that represents the matching addressing method for the received string.
 */
enum addressing_methods determine_addressing_method(char *str) {
    // Immediate Addressing (operand starts with #):
    if (*str == '#' && isNumber(str+1)) return immediate_addressing;
    // Registers Addressing:
    else if (is_register(str)) return register_addressing;
    // (access to) Structs Addressing:
    else if (is_struct_access_call(str)) return struct_addressing;
    // Direct Addressing:
    else if (is_label(str)) return direct_addressing;
    // Otherwise, assuming that there is an invalid addressing method:
    return invalid_addressing;
}


/**
 * A function in charge of handling immediate addressing method.
 * Expected to be called after detecting an immediate addressing method reference (#<integer>).
 * The function converts the received string to an integer, then transforms the integer to binary representation
 * and appends a matching word to the code array.
 * @param code_arr An array of words.
 * @param pc Index in the code_arr.
 * @param ptr A pointer to the relevant word.
 */
void handle_immediate_addressing(word **code_arr, unsigned long *pc, char *ptr) {
    char *res = (char *)malloc(sizeof(char) * WORD_SIZE);
    // Skipping the #:
    if (isNumber(ptr + 1)) {
        snprintf(res, WORD_SIZE, "%s%s", convert_to_x_bit_bin(strtoll(ptr + 1, &ptr, 10), 8), "00");
    } else {
        return;
    }
    // Allocating a new word in the code array & freeing res:
    add_new_word_to_arr(code_arr, pc, res);
    free(res);
}

/**
 * A function in charge of handling an direct addressing method.
 * Expected to be called after detecting a symbol as a part of a command.
 * The function converts the received symbol's address to binary and appends it's binary representation to the code
 * array. If the symbol doesn't exist in the array yet, 10 Xs will be inserted instead.
 * @param head A pointer to the head of the symbols list.
 * @param code_arr An array of words.
 * @param pc Index in the code_arr.
 * @param str A pointer to the string contains a symbol.
 */
void handle_direct_addressing(symbol *head, word **code_arr, unsigned long *pc, char *ptr) {
    char *res = (char *)malloc(sizeof(char) * WORD_SIZE);

    if (list_exists(head, ptr)) {
        symbol *temp = search_list(head, ptr);
        snprintf(res, WORD_SIZE, "%s%s", convert_to_x_bit_bin(temp->address, 8), "10");
        add_new_word_to_arr(code_arr, pc, res);
    } else {
        // Appending a sequence of 10 Xs to the code_arr:
        add_new_word_to_arr(code_arr, pc, create_chars_str('X', WORD_SIZE));
    }
    free(res);
}

/**
 * A function in charge of handling struct access addressing method.
 * Expected to be called when the command ONLY contains an attempt to access a struct field (<struct>.1 / <struct>.2).
 * The function converts the received data to binary and appends 2 matching words to the code array.
 * If the address of the struct is unknown at the time this function is called, 10 Xs will will be inserted.
 * @param head A pointer to the head of the symbols list.
 * @param code_arr An array of words.
 * @param pc Index in the code_arr.
 * @param str A pointer to the string contains the attempt to access a struct field.
 */
void handle_struct_addressing(symbol * head, word **code_arr, unsigned long *pc, const char *str) {
    char *ptr = strdup(str), *token, *st, *ind;
    char *first = (char *)malloc(sizeof(char) * WORD_SIZE), *second = (char *)malloc(sizeof(char) * WORD_SIZE);
    token = strtok(ptr, ".");
    st = strdup(trim(token));
    token = strtok(NULL, ".");
    ind = strdup(trim(token));

    // Handle the first part of the struct as a direct addressing (as a label):
    handle_direct_addressing(head, code_arr, pc, st);
    // Allocating a new word in the code array & freeing first, second & ptr:
    snprintf(second, WORD_SIZE, "%s%s", convert_to_x_bit_bin((size_t)(*ind - '0'), 8), "00");
    add_new_word_to_arr(code_arr, pc, second);
    free(first);
    free(second);
    free(ptr);
}

/**
 * A function in charge of handling register addressing method.
 * Expected to be called when the command ONLY contains registers.
 * The function converts the received data to binary and appends a matching word to the code array.
 * @param code_arr An array of words.
 * @param pc Index in the code_arr.
 * @param ptr A pointer to the first word (if exists).
 * @param ptr2 A pointer to the second word (if exists).
 */
void handle_register_addressing(word **code_arr, unsigned long *pc, const char *ptr, const char *ptr2) {
    char *res = (char *)malloc(sizeof(char) * WORD_SIZE);
    if (ptr != NULL && ptr2 != NULL) {
        snprintf(res, WORD_SIZE, "%s%s%s", convert_to_x_bit_bin(ptr[strlen(ptr) - 1], 4),
                 convert_to_x_bit_bin(ptr2[strlen(ptr2) - 1], 4), "00");
    } else if (ptr != NULL) {
        snprintf(res, WORD_SIZE, "%s%s", convert_to_x_bit_bin(ptr[strlen(ptr) - 1], 8), "00");
    } else {
        free(res);
        return;
    }
    // Allocating a new word in the code array & freeing res:
    add_new_word_to_arr(code_arr, pc, res);
    free(res);
}

void handle_addressing_method(symbol ** head, unsigned long *pc, word **code_arr, char *ptr, enum addressing_methods method) {
    if (method == immediate_addressing)
        handle_immediate_addressing(code_arr, pc, ptr);
    else if (method == direct_addressing)
        handle_direct_addressing(*head, code_arr, pc, ptr);
    else if (method == struct_addressing)
        handle_struct_addressing(*head, code_arr, pc, ptr);
    else if (method == register_addressing)
        handle_register_addressing(code_arr, pc, ptr, NULL);
    else {
//        add_new_issue_to_arr();
    }
}

void encode_two_ops_cmd(char *ptr, symbol ** head, unsigned long *pc, word **code_arr, issue ** errors_array, int *ec, int lc, operator *op) {
    char *cmd = (char *)malloc(WORD_SIZE * sizeof(char));
    char *src = strdup(trim(strtok(ptr, ",")));
    char *dest = strdup(trim(strtok(NULL, ",")));
    enum addressing_methods src_am = determine_addressing_method(src);
    enum addressing_methods dest_am = determine_addressing_method(dest);

    // Convert command to binary and append it to code_arr:
    snprintf(cmd, WORD_SIZE, "%s%s%s%s", convert_to_x_bit_bin(get_operator_index(op->name), 4),
             convert_to_x_bit_bin(dest_am, 2), convert_to_x_bit_bin(src_am, 2), "00");
    add_new_word_to_arr(code_arr, pc, cmd);

    // If we received anything other than immediate_addressing, append an info word for it:
    if (src_am != immediate_addressing) {
        if (list_exists(*head, src)) {
//            add_new_word_to_arr(code_arr, pc, conve);
        }
    }

    // Both are registers:
    if (src_am == register_addressing && dest_am == register_addressing) {
        handle_register_addressing(code_arr, pc, src, dest);
    }

    char *src_addr = convert_to_x_bit_bin(src_am, 2);
    char *dest_addr = convert_to_x_bit_bin(dest_am, 2);
    char *op_addr = convert_to_x_bit_bin(get_operator_index(op->name), 4);

    // Commands are encoded with 00 as A, R, E fields:
    snprintf(cmd, sizeof(cmd), "%s%s%s%s", op_addr, dest_addr, src_addr, "00");
}


void encode_cmd(char *ptr, symbol ** head, unsigned long *pc, word **code_arr, issue ** errors_array, int *ec, int lc) {
    operator *op = get_operator(ptr);
    if (op->ops_count == 2) {
        char res[10];
        char *src = strdup(trim(strtok(ptr, ",")));
        char *dest = strdup(trim(strtok(NULL, ",")));

        char *src_addr = convert_to_x_bit_bin(determine_addressing_method(src), 2);
        char *dest_addr = convert_to_x_bit_bin(determine_addressing_method(dest), 2);
        char *op_addr = convert_to_x_bit_bin(get_operator_index(op->name), 4);
        snprintf(res, sizeof(res), "%s%s%s%s", op_addr, dest_addr, src_addr, "XX");
    }
}


void perform_mov_op(char *ptr, symbol ** head, unsigned long *pc, word **code_arr, issue ** errors_array, int *ec, int lc) {
    operator *op = get_operator(ptr);
    if (op->ops_count == 2) {
        char res[WORD_SIZE];

//        char target[ SIZE ];
// .. ..
//        snprintf( target, sizeof( target ), "%s%s%s", str1, str2, str3 );
//        char *src = strdup(trim(strtok(ptr, ",")));
//        char *dest = strdup(trim(strtok(NULL, ",")));
        char *src_addr = convert_to_x_bit_bin(determine_addressing_method(trim(strtok(ptr, ","))), 2);
        char *dest_addr = convert_to_x_bit_bin(determine_addressing_method(trim(strtok(NULL, ","))), 2);
        char *op_addr = convert_to_x_bit_bin(get_operator_index(op->name), 4);
        snprintf(res, sizeof(res), "%s%s%s%s", op_addr, dest_addr, src_addr, "XX");
        printf("src_addr: %s, dest_addr: %s, op_addr: %s\nfinal: %s\n", src_addr, dest_addr, op_addr, res);
    }

//    ptr = ptr + strlen("mov") + 1;
//    char *src = strdup(trim(strtok(ptr, ",")));
//    char *dest = strdup(trim(strtok(NULL, ",")));
//
//
//    char *src_addr = convert_to_x_bit_bin(determine_addressing_method(src), 2);
//    char *dest_addr = convert_to_x_bit_bin(determine_addressing_method(dest), 2);
//    char *opcode_addr = determine_addressing_method()

//    if (list_exists(*head, src)) handle_symbol_in_op(src);
//    if (strchr(src, '.')) handle_struct_in_op(src);
//    if (is_register(src)) handle_register_in_op(src);

//    printf("src: %s, dest: %s\n", src, dest);
//    printf("srcAM: %s, destAM: %s\n", convert_to_x_bit_bin(src_am, 2), convert_to_x_bit_bin(dest_am, 2));



//    ptr = ptr + strlen(".entry") + 1;
//    char *symbolName = NULL, *token = strtok(ptr, ",");
//    // Appending each symbol to the symbols list:
//    while (token != NULL) {
//        symbolName = (char *)strdup(token);
//        append_unique(head, symbolName, 0, symbol_entry, errors_array, ec, lc);
//        token = strtok(NULL, ",");
//    }
//
//
//    while (*ptr) { // While there are more characters to process...
//        if ( isdigit(*ptr) || ( (*ptr=='-'||*ptr=='+') && isdigit(*(ptr+1)) )) {
//            word temp;
//            convert_to_10_bit_bin(strtoll(ptr, &ptr, 10), temp.value);
//            temp.address = (*dc);
//            *data_arr = (word *)realloc(*data_arr, sizeof(word) * ((*dc) + 1));
//            (*data_arr)[(*dc)++] = temp;
//            ptr = strchr(ptr, ',') ? strchr(ptr, ',') : strchr(ptr, '\0');
//        } else {
//            ptr++;
//        }
//    }
}