#include "operators.h"
#include "../utils/string_utils.h"
#include "../utils/conversions.h"
#include "../utils/validators.h"
#include "../utils/addr_methods.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * An array of operators.
 * Each operator contains a string representing an operation & the amount of operands that it expects to receive.
 */
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

/**
 * An array of additional data associated with each operator.
 * Note: Since we're using this array by matching index from the first ops array, the op field here is redundant but I chose to leave it here anyways in case someday someone chooses to associate
 */
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
 * A function in charge of encoding a command without any operands (for example - hlt).
 * @param ptr A pointer to a line from the "src code" containing an operator that we would like to encode.
 * @param head The head of a symbols ll.
 * @param ent_table_head The head of an entities ll.
 * @param pc Index in the code array.
 * @param code_arr An array that contains the code segment.
 * @param errors_array An array of errors.
 * @param ec Index in the errors array.
 * @param lc Line number from the "src code".
 * @param op An operator to encode the command for.
 * @param rt run type (first / second).
 */
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


/**
 * A function in charge of encoding a command with exactly 1 operand (for example - jmp).
 * @param ptr A pointer to a line from the "src code" containing an operator that we would like to encode.
 * @param head The head of a symbols ll.
 * @param ent_table_head The head of an entities ll.
 * @param pc Index in the code array.
 * @param code_arr An array that contains the code segment.
 * @param errors_array An array of errors.
 * @param ec Index in the errors array.
 * @param lc Line number from the "src code".
 * @param op An operator to encode the command for.
 * @param rt run type (first / second).
 */
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


/**
 * A function in charge of encoding a command with exactly 2 operand (for example - mov).
 * @param ptr A pointer to a line from the "src code" containing an operator that we would like to encode.
 * @param head The head of a symbols ll.
 * @param ent_table_head The head of an entities ll.
 * @param pc Index in the code array.
 * @param code_arr An array that contains the code segment.
 * @param errors_array An array of errors.
 * @param ec Index in the errors array.
 * @param lc Line number from the "src code".
 * @param op An operator to encode the command for.
 * @param rt run type (first / second).
 */
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


/**
 * A function in charge of encoding the received command by calling a matching handling function based on the amount
 * of operands associated with the received operator.
 * @param ptr A pointer to a line from the "src code" containing an operator that we would like to encode.
 * @param head The head of a symbols ll.
 * @param ent_table_head The head of an entities ll.
 * @param pc Index in the code array.
 * @param code_arr An array that contains the code segment.
 * @param errors_array An array of errors.
 * @param ec Index in the errors array.
 * @param lc Line number from the "src code".
 * @param rt run type (first / second).
 */
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