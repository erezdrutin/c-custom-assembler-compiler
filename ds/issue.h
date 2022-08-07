#ifndef MAMAN14_ISSUE_H
#define MAMAN14_ISSUE_H

typedef struct Issue {
    char * error;
    unsigned short line;
} issue;

void print_issues(issue * issues_arr, int len);
void add_new_issue_to_arr(issue ** errors_array, int *ec, int lc, char * error);
#endif