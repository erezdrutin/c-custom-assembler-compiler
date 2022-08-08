#include "addr_methods.h"
#include "string_utils.h"
#include "conversions.h"
#include "validators.h"
#include <stdio.h>
#include <string.h>

/**
 * A function in charge of checking whether the received string is a register or not.
 * @param str A string representing an operand (probably).
 * @return True (1) if the string is a register or False (0) if not.
 */
int is_register(char *str) {
    int i;
    /* Initializing with '\0' at the end (therefore we need 3 chars = strlen("r%d")): */
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

/**
 * A function in charge of checking whether the received label is in the "blacklist" of words for the assembler.
 * The blacklist contains the .string, .data, .struct, .entry & .extern instructions, while also containing the
 * operands, registers & macro definition words.
 * @param label The label that we would like to check if its in the blacklist or not.
 * @return True (1) if it is in the blacklist or False (0) if it isn't.
 */
int is_label_in_blacklist(char *label) {
    if (!strcmp(".string", label) || !strcmp(".data", label) || !strcmp(".struct", label)
        || !strcmp(".entry", label) || !strcmp(".extern", label) || !strcmp("macro", label)
        || !strcmp("endmacro", label) || is_register(label) || is_label_an_op(label))
        return 1;
    return 0;
}

/**
 * A function in charge of validating that a label is valid.
 * A valid label starts with an alphabet letter which is either small or capital, followed by a sequence of characters
 * that are alphanumeric. In addition, a valid label may consist of up to 30 characters.
 * @param str A string to check if it matches the definition of a valid label.
 * @return True (1) / False (0).
 */
int is_label(char *str) {
    /* Starts with alphabet (small/CAP), followed by a sequence of alphabet (small/CAP) letters / digits,
     * and is up to 30 chars. */
    int i;
    /* Validate that the label starts with an alphabet letter: */
    if (!((*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z'))
        || strlen(str) > 30 || is_label_in_blacklist(str)) return 0;
    /* Validating that every other letter is alphanumeric: */
    for (i = 1; i <= strlen(str); i++) {
        if (!((str[i] >= '0' && str[i] <= '9') || (*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z'))) {
            return 0;
        }
    }
    return 1;
}

/**
 * A function in charge of checking whether the received string is a struct or not.
 * @param str A string representing an operand (probably)
 * @return True (1) if the string is a struct or False (0) if not.
 */
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


/**
 * A function in charge of determining the appropriate addressing method for the received string (operand).
 * @param str A string that represents an operand.
 * @return A value that represents the matching addressing method for the received string.
 */
enum addressing_methods determine_addressing_method(char *str) {
    /* Immediate Addressing (operand starts with #): */
    if (*str == '#' && is_number(str + 1)) return immediate_addressing;
    /* Registers Addressing: */
    else if (is_register(str)) return register_addressing;
    /* (access to) Structs Addressing: */
    else if (is_struct_access_call(str)) return struct_addressing;
    /* Direct Addressing: */
    else if (is_label(str)) return direct_addressing;
    /* Otherwise, assuming that there is an invalid addressing method: */
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
 * @param rt The run type (first run / second run).
 */
void handle_immediate_addressing(word **code_arr, unsigned long *pc, char *ptr, enum run_type rt) {
    char *res = (char *)malloc(sizeof(char) * WORD_SIZE);
    /* Skipping the #: */
    if (is_number(ptr + 1)) {
        snprintf(res, WORD_SIZE, "%s%s", convert_to_x_bit_bin(strtoll(ptr + 1, &ptr, 10), 8), "00");
    } else {
        return;
    }
    /* Allocating a new word in the code array & freeing res: */
    add_or_update_word_in_arr(code_arr, pc, res, rt);
    free(res);
}

/**
 * A function in charge of handling direct addressing method.
 * Expected to be called after detecting a symbol as a part of a command.
 * The function converts the received symbol's address to binary and appends it's binary representation to the code
 * array. If the symbol doesn't exist in the array yet, 10 Xs will be inserted instead.
 * @param head The head of a symbols ll.
 * @param ent_table_head The head of an entities ll.
 * @param code_arr An array that contains the code segment.
 * @param pc Index in the code array.
 * @param ptr A pointer to a line from the "src code".
 * @param rt The run type (first run / second run).
 * @param errors_arr An array of errors.
 * @param ec Index in the errors array.
 * @param lc The number of the line that we're currently handling from the "src file".
 */
void handle_direct_addressing(symbol *head, symbol *ent_table_head, word **code_arr, unsigned long *pc, char *ptr, enum run_type rt, issue ** errors_arr, int *ec, int lc) {
    char *res = (char *)malloc(sizeof(char) * WORD_SIZE);

    symbol *temp = search_list(head, ptr);
    if (temp != NULL) {
        /* Append value to words array as external: */
        if (temp->kind == symbol_extern || temp->kind == symbol_entry) {
            append_unique_symbol_from_op(&head, &ent_table_head, ptr, (unsigned int)*pc, symbol_code, errors_arr, ec, lc);
            snprintf(res, WORD_SIZE, "%s%s", convert_to_x_bit_bin(temp->address, 8), "01");
            add_or_update_word_in_arr(code_arr, pc, res, rt);
        }
        else {
            /* Append value to words array as relocatable: */
            snprintf(res, WORD_SIZE, "%s%s", convert_to_x_bit_bin(temp->address, 8), "10");
            add_or_update_word_in_arr(code_arr, pc, res, rt);
        }
    } else {
        /* Appending a sequence of 10 Xs to the code_arr: */
        add_or_update_word_in_arr(code_arr, pc, create_chars_str('X', WORD_SIZE), rt);
    }

    free(res);
}

/**
 * A function in charge of handling struct access addressing method.
 * Expected to be called when the command ONLY contains an attempt to access a struct field (<struct>.1 / <struct>.2).
 * The function converts the received data to binary and appends 2 matching words to the code array.
 * If the address of the struct is unknown at the time this function is called, 10 Xs will will be inserted.
 * @param head The head of a symbols ll.
 * @param ent_table_head The head of an entities ll.
 * @param code_arr An array that contains the code segment.
 * @param pc Index in the code array.
 * @param str A pointer to a line from the "src code".
 * @param rt The run type (first run / second run).
 * @param errors_arr An array of errors.
 * @param ec Index in the errors array.
 * @param lc The number of the line that we're currently handling from the "src file".
 */
void handle_struct_addressing(symbol * head, symbol *ent_table_head, word **code_arr, unsigned long *pc, const char *str, enum run_type rt, issue ** errors_arr, int *ec, int lc) {
    char *ptr = strdup(str), *token, *st, *ind;
    char *second = (char *)malloc(sizeof(char) * WORD_SIZE);
    token = strtok(ptr, ".");
    st = strdup(trim(token));
    token = strtok(NULL, ".");
    ind = strdup(trim(token));

    /* Handle the first part of the struct as a direct addressing (as a label): */
    handle_direct_addressing(head, ent_table_head, code_arr, pc, st, rt, errors_arr, ec, lc);
    /* Allocating a new word in the code array & freeing first, second & ptr: */
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
 * @param rt The run type (first run / second run).
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
    /* Allocating a new word in the code array & freeing res: */
    add_or_update_word_in_arr(code_arr, pc, res, rt);
    free(res);
}

/**
 * A function in charge of handling the received addressing method by calling the relevant function.
 * @param head The head of a symbols ll.
 * @param ent_table_head The head of an entities ll.
 * @param pc Index in the code array.
 * @param code_arr An array that contains the code segment.
 * @param ptr A pointer to a line from the "src code".
 * @param method The addressing method associated with the line.
 * @param ot The operand type (handling register addressing differently for src / dest ops).
 * @param rt The run type (first run / second run).
 * @param errors_array An array of errors.
 * @param ec Index in the errors array.
 * @param lc The number of the line that we're currently handling from the "src file".
 */
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
}