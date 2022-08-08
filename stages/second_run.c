#include "second_run.h"
#include <string.h>
#include "../utils/handlers.h"
#include "../utils/validators.h"
#include "../utils/string_utils.h"

/**
 * A function in charge of handling a given line from the "src code" (the contents of the
 * original file after expanding the relevant macros).
 * This function will call the relevant functions with the relevant parameters in order to handle different data types.
 * In addition, if we detect any syntax issues in the "src code", we will skip them as we already handled them in the
 * first run. However, missing labels and other more "complex" issues will be detected an added to the issues array at
 * this "phase" of the process.
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
void handle_line_second_run(symbol **head, symbol **ent_table_head, unsigned long *mc, word **mem_arr, int lc, const char *line, issue **errors_array, int *ec) {
    char *ptr = (char *)line;
    enum run_type rt = second_run_type;
    enum data_type curDataType = parse_str(ptr);
    /* Skipping any symbols (because we already declared them in the 1st run): */
    if (curDataType == symbol_dt)
        ptr = strchr(ptr, ':') + 1;

    /* Skipping invalid lines: */
    curDataType = parse_str(ptr);
    if (!validate_str(ptr, curDataType)) return;


    /* Only handling code "segment" (operations related fields that weren't initialized in the 1st run): */
    if (curDataType == op)
        handle_operator(ptr, head, ent_table_head, mc, mem_arr, errors_array, ec, lc, rt);
    else if (curDataType == sstring || curDataType == sdata || curDataType == sstruct || curDataType == iextern || curDataType == ientry)
        return;
}

/**
 * A function in charge of performing the "second run" phase of the process.
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
void second_run(char *content, symbol **head, symbol **ent_table_head, word **mem_arr, unsigned long *mc, issue **errors_array, int *ec) {
    char * curLine = content, *nextLine = NULL;
    int lc = 1; /* Lines counter */
    while(curLine)
    {
        nextLine = strchr(curLine, '\n');
        /* Temporarily terminate the current line: */
        if (nextLine) *nextLine = '\0';
        /* Trim whitespaces: */
        curLine = trim(curLine);
        /* Skipping comments in assembler file: */
        if (*curLine != ';'){
            if (!is_empty(curLine)) handle_line_second_run(head, ent_table_head, mc, mem_arr, lc, curLine, errors_array, ec);
        }
        /* Then restore newline-char: */
        if (nextLine) *nextLine = '\n';
        curLine = nextLine ? (nextLine+1) : NULL;
        lc++;
    }
}