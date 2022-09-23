/**
 * Author: Erez Drutin
 * Date: 07.08.2022
 * Purpose: Contain global definitions for structures & enums used throughout the project.
 */

#ifndef MAMAN14_GLOBALS_H
#define MAMAN14_GLOBALS_H
enum run_type { first_run_type, second_run_type };
enum data_type { symbol_dt, sdata, sstring, sstruct, iextern, ientry, op, invalid };
enum addressing_methods {  immediate_addressing = 0, direct_addressing = 1, struct_addressing = 2,
    register_addressing = 3, invalid_addressing = -1};
enum operands_count_options { zero_ops = 0, one_op = 1, two_ops = 2 };
enum operand_type { src_operand, dest_operand };

typedef struct Operator {
    char * name;
    enum operands_count_options ops_count;
} operator;

typedef struct OperatorMetaData {
    operator * op;
    enum addressing_methods src_am[4];
    enum addressing_methods dest_am[4];
} operatorMetaData;

#endif
