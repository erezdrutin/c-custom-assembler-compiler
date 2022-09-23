cmake_minimum_required(VERSION 3.21)
project (maman14 C)

set(CMAKE_C_STANDARD 90)

add_executable(maman14 main.c files.c files.h string_utils.c string_utils.h pre_assembly.c pre_assembly.h macros.c macros.h assembler.c assembler.h symbol.c symbol.h conversions.c conversions.h word.c word.h operators.c operators.h issue.c issue.h globals.h first_run.c first_run.h handlers.c handlers.h validators.c validators.h second_run.c second_run.h addr_methods.c addr_methods.h)
