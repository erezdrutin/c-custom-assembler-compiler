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
        {"not", one_op},
        {"clr", one_op},
        {"lea", two_ops},
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

operatorMetaData opsMetaData[] = {
        {&ops[0], {immediate_addressing, direct_addressing, struct_addressing, register_addressing}, {direct_addressing, struct_addressing, register_addressing}},
        {&ops[1], {immediate_addressing, direct_addressing, struct_addressing, register_addressing}, {immediate_addressing,direct_addressing, struct_addressing, register_addressing}},
        {&ops[2], {immediate_addressing, direct_addressing, struct_addressing, register_addressing}, {direct_addressing, struct_addressing, register_addressing}},
        {&ops[3], {immediate_addressing, direct_addressing, struct_addressing, register_addressing}, {direct_addressing, struct_addressing, register_addressing}},
        {&ops[4], {invalid_addressing}, {direct_addressing, struct_addressing, register_addressing}},
        {&ops[5], {invalid_addressing}, {direct_addressing, struct_addressing, register_addressing}},
        {&ops[6], {direct_addressing, struct_addressing}, {direct_addressing, struct_addressing, register_addressing}},
        {&ops[7], {invalid_addressing}, {direct_addressing, struct_addressing, register_addressing}},
        {&ops[8], {invalid_addressing}, {direct_addressing, struct_addressing, register_addressing}},
        {&ops[9], {invalid_addressing}, {direct_addressing, struct_addressing, register_addressing}},
        {&ops[10], {invalid_addressing}, {direct_addressing, struct_addressing, register_addressing}},
        {&ops[11], {invalid_addressing}, {direct_addressing, struct_addressing, register_addressing}},
        {&ops[12], {invalid_addressing}, {immediate_addressing, direct_addressing, struct_addressing, register_addressing}},
        {&ops[13], {invalid_addressing}, {direct_addressing, struct_addressing, register_addressing}},
        {&ops[14], {invalid_addressing}, {invalid_addressing}},
        {&ops[15], {invalid_addressing}, {invalid_addressing}}
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
operator * get_operator(char *str) {
    int len = (size_t) sizeof(ops) / sizeof(ops[0]), i;
    for (i = 0; i < len; ++i) {
        if (strstr(str, ops[i].name) || !strcmp(str, ops[i].name)) {
            return &ops[i];
        }
    }
    return NULL;
}

/**
 * A function in charge of fetching the matching meta data for the received operator.
 * The association is done by the received operator.
 * @param ptr A pointer to an operator struct.
 * @return A pointer to a meta data struct matching the received operator.
 */
operatorMetaData * get_operator_meta_data(operator *ptr) {
    int len = (size_t) sizeof(opsMetaData) / sizeof(opsMetaData[0]), i;
    for (i = 0; i < len; i++) {
        if (!strcmp(ptr->name, ops[i].name)) {
            return &opsMetaData[i];
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
int validate_operator_usage_in_str(const char *str) {
    // Duplicating the string & counting how many args were passed:
    char *ptr = trim((char *)str), *token = NULL;
    int args = 0, res;
    operator *temp = get_operator(ptr);
    // If we received an operator in which we expect to get 0 parameters:
    if (temp->ops_count == 0) {
        if (!strcmp(ptr, temp->name)) return 1;
        return 0;
    }
    // Fetching "first" part of string & Counting how many comma separated parts are in the string:
    ptr = strdup(trim(ptr + strlen(temp->name)));
    token = strtok(ptr, ",");
    if (temp -> ops_count == 1 && token != NULL) args++;
    while( temp->ops_count == 2 && token != NULL) {
        args++;
        token = strtok(NULL, " ");
    }

    free(ptr);
    // Validate that the amount of args matches our expectation + that the received string != the found operator:
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
        // Validating addressing methods for both src & dest:
        if (op_md->src_am[i] == src_am)
            src_flag = 1;
        if (op_md->dest_am[i] == dest_am)
            dest_flag = 1;
        // If we expected an invalid addressing method but received another addressing method - return 0
        // (initializing src with 0 is enough in this case):
        if ((op_md->src_am[i] == invalid_addressing && src_am != invalid_addressing) || (op_md->dest_am[i] == invalid_addressing && dest_am != invalid_addressing)) {
            src_flag = 0;
            break;
        }
    }
    return src_flag && dest_flag;
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
void handle_immediate_addressing(word **code_arr, unsigned long *pc, char *ptr, enum run_type rt) {
    char *res = (char *)malloc(sizeof(char) * WORD_SIZE);
    // Skipping the #:
    if (isNumber(ptr + 1)) {
        snprintf(res, WORD_SIZE, "%s%s", convert_to_x_bit_bin(strtoll(ptr + 1, &ptr, 10), 8), "00");
    } else {
        return;
    }
    // Allocating a new word in the code array & freeing res:
    add_or_update_word_in_arr(code_arr, pc, res, rt);
    free(res);
}

/**
 * A function in charge of handling an direct addressing method.
 * Expected to be called after detecting a symbol as a part of a command.
 * The function converts the received symbol's address to binary and appends it's binary representation to the code
 * array. If the symbol doesn't exist in the array yet, 10 Xs will be inserted instead.
 * If a symbol already exists, assuming that we're referencing an external symbol occurrence, therefore appending the
 * symbol with an external symbol reference.
 * @param head A pointer to the head of the symbols list.
 * @param code_arr An array of words.
 * @param pc Index in the code_arr.
 * @param str A pointer to the string contains a symbol.
 */
void handle_direct_addressing(symbol *head, symbol *ent_table_head, word **code_arr, unsigned long *pc, char *ptr, enum run_type rt, issue ** errors_arr, int *ec, int lc) {
    char *res = (char *)malloc(sizeof(char) * WORD_SIZE);

    symbol *temp = search_list(head, ptr);
    if (temp != NULL) {
        // Append value to words array as external:
        if (temp->kind == symbol_extern || temp->kind == symbol_entry) {
            append_unique(&head, &ent_table_head, ptr, (unsigned int)*pc, symbol_code, errors_arr, ec, lc);
            snprintf(res, WORD_SIZE, "%s%s", convert_to_x_bit_bin(temp->address, 8), "01");
            add_or_update_word_in_arr(code_arr, pc, res, rt);
        }
        else {
            // Append value to words array as relocatable:
            snprintf(res, WORD_SIZE, "%s%s", convert_to_x_bit_bin(temp->address, 8), "10");
            add_or_update_word_in_arr(code_arr, pc, res, rt);
        }
    } else {
        // Appending a sequence of 10 Xs to the code_arr:
        add_or_update_word_in_arr(code_arr, pc, create_chars_str('X', WORD_SIZE), rt);
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
void handle_struct_addressing(symbol * head, symbol *ent_table_head, word **code_arr, unsigned long *pc, const char *str, enum run_type rt, issue ** errors_arr, int *ec, int lc) {
    char *ptr = strdup(str), *token, *st, *ind;
    char *second = (char *)malloc(sizeof(char) * WORD_SIZE);
    token = strtok(ptr, ".");
    st = strdup(trim(token));
    token = strtok(NULL, ".");
    ind = strdup(trim(token));

    // Handle the first part of the struct as a direct addressing (as a label):
    handle_direct_addressing(head, ent_table_head, code_arr, pc, st, rt, errors_arr, ec, lc);
    // Allocating a new word in the code array & freeing first, second & ptr:
    snprintf(second, WORD_SIZE, "%s%s", convert_to_x_bit_bin((size_t)(*ind - '0'), 8), "00");
    add_or_update_word_in_arr(code_arr, pc, second, rt);
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
void handle_register_addressing(word **code_arr, unsigned long *pc, const char *ptr, const char *ptr2, enum run_type rt) {
    char *res = (char *)malloc(sizeof(char) * WORD_SIZE);
    if (ptr != NULL && ptr2 != NULL) {
        snprintf(res, WORD_SIZE, "%s%s%s", convert_to_x_bit_bin(ptr[strlen(ptr) - 1] - '0', 4),
                 convert_to_x_bit_bin(ptr2[strlen(ptr2) - 1] - '0', 4), "00");
    } else if (ptr != NULL) {
        snprintf(res, WORD_SIZE, "%s%s%s", convert_to_x_bit_bin(ptr[strlen(ptr) - 1] - '0', 4), "0000", "00");
    } else if (ptr2 != NULL) {
        snprintf(res, WORD_SIZE, "%s%s%s", "0000", convert_to_x_bit_bin(ptr2[strlen(ptr2) - 1] - '0', 4), "00");
    }
    else {
        free(res);
        return;
    }
    // Allocating a new word in the code array & freeing res:
    add_or_update_word_in_arr(code_arr, pc, res, rt);
    free(res);
}

void handle_addressing_method(symbol ** head, symbol **ent_table_head, unsigned long *pc, word **code_arr, char *ptr, enum addressing_methods method, enum operand_type ot, enum run_type rt, issue ** errors_array, int *ec, int lc) {
    if (method == immediate_addressing)
        handle_immediate_addressing(code_arr, pc, ptr, rt);
    else if (method == direct_addressing)
        handle_direct_addressing(*head, *ent_table_head, code_arr, pc, ptr, rt, errors_array, ec, lc);
    else if (method == struct_addressing)
        handle_struct_addressing(*head, *ent_table_head, code_arr, pc, ptr, rt, errors_array, ec, lc);
    else if (method == register_addressing) {
        if (ot == src_operand) handle_register_addressing(code_arr, pc, ptr, NULL, rt);
        else handle_register_addressing(code_arr, pc, NULL, ptr, rt);
    }
    else {
//        add_new_issue_to_arr();
    }
}

void encode_opless_cmd(char *ptr, symbol ** head, unsigned long *pc, word **code_arr, issue ** errors_array, int *ec, int lc, operator *op, enum run_type rt) {
    char *cmd = (char *)malloc(WORD_SIZE * sizeof(char));

    if (!validate_operator_am_usage(invalid_addressing, invalid_addressing, get_operator_meta_data(op))) {
        free(cmd);
        // Append to errors array as bad usage of operator (+2 chars to include the 3 chars operator + '/0')
        size_t errLen = strlen("Invalid usage of addressing methods combined with the operator %s.") + 2;
        char *error = (char *)malloc(errLen * sizeof(char));
        snprintf(error, errLen, "Invalid usage of addressing methods combined with the operator %s.", op->name);
        add_new_issue_to_arr(errors_array, ec, lc, error);
        return;
    }

    // Convert operand addressing method & operation addressing method to bin:
    char *operator_addr = convert_to_x_bit_bin(get_operator_index(op->name), 4);

    // Commands are encoded with 00 as A, R, E fields + SRC is encoded as "00" since it's missing:
    snprintf(cmd, WORD_SIZE, "%s%s%s%s", operator_addr, "00", "00", "00");
    add_or_update_word_in_arr(code_arr, pc, cmd, rt);
}


void encode_one_op_cmd(char *ptr, symbol ** head, symbol **ent_table_head, unsigned long *pc, word **code_arr, issue ** errors_array, int *ec, int lc, operator *op, enum run_type rt) {
    char *cmd = (char *)malloc(WORD_SIZE * sizeof(char));
    char *operand = strdup(trim(ptr + strlen(op->name)));
    enum addressing_methods operand_am = determine_addressing_method(operand);
    if (!validate_operator_am_usage(invalid_addressing, operand_am, get_operator_meta_data(op))) {
//        free(cmd);
        free(operand);
        // Append to errors array as bad usage of operator (+2 chars to include the 3 chars operator + '/0')
        size_t errLen = strlen("Invalid usage of addressing methods combined with the operator %s.") + 2;
        char *error = (char *)malloc(errLen * sizeof(char));
        snprintf(error, errLen, "Invalid usage of addressing methods combined with the operator %s.", op->name);
        add_new_issue_to_arr(errors_array, ec, lc, error);
        return;
    }

    // Convert operand addressing method & operation addressing method to bin:
    char *operand_addr = convert_to_x_bit_bin(operand_am, 2);
    char *operator_addr = convert_to_x_bit_bin(get_operator_index(op->name), 4);

    // Commands are encoded with 00 as A, R, E fields + SRC is encoded as "00" since it's missing:
    snprintf(cmd, WORD_SIZE, "%s%s%s%s", operator_addr, "00", operand_addr, "00");
    add_or_update_word_in_arr(code_arr, pc, cmd, rt);

    // Handle relevant addressing method for the current operation:
    handle_addressing_method(head, ent_table_head, pc, code_arr, operand, operand_am, src_operand, rt, errors_array, ec, lc);
}


void encode_two_ops_cmd(char *ptr, symbol ** head, symbol **ent_table_head, unsigned long *pc, word **code_arr, issue ** errors_array, int *ec, int lc, operator *op, enum run_type rt) {
    char *cmd = (char *)malloc(WORD_SIZE * sizeof(char));
    char *src = strdup(trim(strtok(ptr + strlen(op->name), ",")));
    char *dest = strdup(trim(strtok(NULL, ",")));
    enum addressing_methods src_am = determine_addressing_method(src);
    enum addressing_methods dest_am = determine_addressing_method(dest);

    if (!validate_operator_am_usage(src_am, dest_am, get_operator_meta_data(op))) {
        free(cmd);
        free(src);
        free(dest);
        // Append to errors array as bad usage of operator (+2 chars to include the 3 chars operator + '/0')
        size_t errLen = strlen("Invalid usage of addressing methods combined with the operator %s.");
        char *error = (char *)malloc(errLen * sizeof(char));
        snprintf(error, errLen, "Invalid usage of addressing methods combined with the operator %s.", op->name);
        add_new_issue_to_arr(errors_array, ec, lc, error);
        return;
    }

    char *src_addr = convert_to_x_bit_bin(src_am, 2);
    char *dest_addr = convert_to_x_bit_bin(dest_am, 2);
    char *op_addr = convert_to_x_bit_bin(get_operator_index(op->name), 4);

    // Commands are encoded with 00 as A, R, E fields:
    snprintf(cmd, WORD_SIZE, "%s%s%s%s", op_addr, src_addr, dest_addr, "00");
    add_or_update_word_in_arr(code_arr, pc, cmd, rt);

    if (src_am == dest_am && src_am == register_addressing) {
        handle_register_addressing(code_arr, pc, src, dest, rt);
    } else {
        handle_addressing_method(head, ent_table_head, pc, code_arr, src, src_am, src_operand, rt, errors_array, ec, lc);
        handle_addressing_method(head, ent_table_head, pc, code_arr, dest, dest_am, dest_operand, rt, errors_array, ec, lc);
    }
}


void encode_cmd(char *ptr, symbol ** head, symbol **ent_table_head, unsigned long *pc, word **code_arr, issue ** errors_array, int *ec, int lc, enum run_type rt) {
    operator *op = get_operator(ptr);
    ptr = trim(ptr);
    if (op->ops_count == two_ops) {
        encode_two_ops_cmd(ptr, head, ent_table_head, pc, code_arr, errors_array, ec, lc, op, rt);
    } else if (op->ops_count == one_op) {
        encode_one_op_cmd(ptr, head, ent_table_head, pc, code_arr, errors_array, ec, lc, op, rt);
    } else {
        encode_opless_cmd(ptr, head, pc, code_arr, errors_array, ec, lc, op, rt);
    }
}