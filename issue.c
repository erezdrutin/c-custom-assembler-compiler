#include "issue.h"
#include <stdlib.h>
#include <stdio.h>

void print_issues(char *fileName, issue * issues_arr, int len) {
    int i;
    printf("~~~~ ERRORS IN FILE %s: ~~~~\n", fileName);
    for (i = 0; i < len; i++) printf("error at line %d, %s\n", issues_arr[i].line, issues_arr[i].error);
}

/**
 * A function in charge of dynamically allocating memory in order to append new issues to the
 * errors array based on the received parameters.
 * @param errors_array An array of issues.
 * @param ec The errors counter.
 * @param lc The lines counter.
 * @param error An error string to append to the errors array.
 */
void add_new_issue_to_arr(issue ** errors_array, int *ec, int lc, char * error) {
    issue curIssue;
    curIssue.error = error;
    curIssue.line = lc;
    *errors_array = (issue *) realloc(*errors_array, sizeof(issue)*((*ec) + 1));
    (*errors_array)[(*ec)++] = curIssue;
}