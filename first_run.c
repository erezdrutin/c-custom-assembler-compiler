#include "first_run.h"
#include "handlers.h"
#include "validators.h"
#include "string_utils.h"
#include <string.h>

/**
 * A function in charge of handling a given line from the "src code" (the contents of the
 * original file after expanding the relevant macros).
 * This function will call the relevant functions with the relevant parameters in order to handle different data types.
 * In addition, if we detect any syntax issues in the "src code", we will append them to the errors array.
 * @param head The head of a symbols ll.
 * @param ent_table_head The head of an entities ll.
 * @param pc Index in the code array.
 * @param code_arr An array that contains the code segment.
 * @param dc Index in the data array.
 * @param data_arr An array the contains the data segment.
 * @param errors_array An array of errors.
 * @param ec Index in the errors array.
 * @param lc Number of line in "src code".
 */
void handle_line_first_run(symbol **head, symbol **ent_table_head, unsigned long *pc, word **code_arr, unsigned long *dc, const char *line, word **data_arr, issue **errors_array, int *ec, int lc) {
    enum data_type curDataType;
    char *ptr = (char *)line;
    enum run_type rt = first_run_type;

    /* Handle symbols (if there are any): */
    if (!handle_symbol(head, ent_table_head, *pc, *dc, &ptr, errors_array, ec, lc)) return;

    curDataType = parse_str(ptr);
    if (!validate_str(ptr, curDataType)) {
        add_new_issue_to_arr(errors_array, ec, lc, "Invalid instruction.");
        return;
    }

    /* Fetch the actual data type of the row (after "skipping" the symbol_type part): */
    if (curDataType == sstring)
        handle_string(ptr, dc, data_arr, rt);
    else if (curDataType == sdata)
        handle_data(ptr, dc, data_arr, rt);
    else if (curDataType == sstruct) {
        /* Handling data up to the first comma & string after the first comma: */
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

/**
 * A function in charge of performing the "first run" phase of the process.
 * In this function, we're calling the handle_line function for each line from the received "content".
 * @param content The contents of a file that we would like to process.
 * @param head The head of a symbols ll.
 * @param ent_table_head The head of an entities ll.
 * @param pc Index in the code array.
 * @param code_arr An array that contains the code segment.
 * @param dc Index in the data array.
 * @param data_arr An array the contains the data segment.
 * @param errors_array An array of errors.
 * @param ec Index in the errors array.
 */
void first_run(const char *content, symbol **head, symbol **ent_table_head, unsigned long *pc, word **code_arr, unsigned long *dc, word **data_arr, issue **errors_array, int *ec) {
    char *ptr = strdup(content), *curLine = ptr, *nextLine = NULL;
    /* Lines counter */
    int lc = 1;

    while(curLine)
    {
        nextLine = strchr(curLine, '\n');
        /* Temporarily terminate the current line */
        if (nextLine) *nextLine = '\0';
        /* Trim whitespaces */
        curLine = trim(curLine);
        /* Skipping comments in assembler file: */
        if (*curLine != ';'){
            if (!is_empty(curLine)) handle_line_first_run(head, ent_table_head, pc, code_arr, dc, curLine, data_arr, errors_array, ec, lc);
        }
        /* Restore newline character */
        if (nextLine) *nextLine = '\n';
        curLine = nextLine ? (nextLine+1) : NULL;
        lc++;
    }
    free(ptr);
}