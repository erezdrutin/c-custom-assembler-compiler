#ifndef MAMAN14_ASSEMBLER_H
#define MAMAN14_ASSEMBLER_H
#include "../ds/symbol.h"
#include "../utils/globals.h"
symbol* generate_symbols(char* content);
void assemble_machine_code(char *content);
#endif //MAMAN14_ASSEMBLER_H
