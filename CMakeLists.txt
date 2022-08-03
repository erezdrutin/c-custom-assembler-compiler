cmake_minimum_required(VERSION 3.21)
project (maman14 C)

set(CMAKE_C_STANDARD 90)

add_executable(maman14 main.c utils/files.c utils/files.h utils/string_utils.c utils/string_utils.h stages/pre_assembly.c stages/pre_assembly.h utils/macros.c utils/macros.h stages/assembler.c stages/assembler.h ds/symbol.c ds/symbol.h utils/conversions.c utils/conversions.h ds/word.c ds/word.h utils/dates.c utils/dates.h stages/assembler_old.c stages/assembler_old.h ds/operators.c ds/operators.h ds/issue.c ds/issue.h)
