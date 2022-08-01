#ifndef MAMAN14_OPERATORS_H
#define MAMAN14_OPERATORS_H
enum operands_count_options { zero_ops = 0, one_op = 1, two_ops = 2 };
typedef struct Operator {
    char * name;
    enum operands_count_options ops_count;
} operator;
extern operator ops[];
int get_operator_index(char *str);
operator * get_operator (char* str);
int validate_operator_usage(const char *str);
#endif //MAMAN14_OPERATORS_H
