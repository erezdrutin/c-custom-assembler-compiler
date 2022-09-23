#include "files.h"
#include "string_utils.h"
#include "pre_assembly.h"
#include "assembler.h"
#include <stdio.h>
#include <string.h>

/**
 * Processing the arguments passed via the CLI.
 * For each file passed via the CLI, we will perform the different assembler process to turn them into machine code.
 * @param argc The amount of arguments passed via the CLI (including the path to execute the assembler).
 * @param argv An array that contains the arguments that were passed via the CLI.
 */
void process_args(const int* argc, char** argv) {
    int i;
    char *content = NULL;
    for (i=1; i<*argc; i++) {
        /* Perform pre-assembly: */
        if (does_file_exist(str_cat_copy(argv[i], ".as"))) {
            pre_assembly(str_cat_copy(argv[i], ".as"));
            /* Perform assembly: */
            content = read_file(str_cat_copy(argv[i], ".am"));
            if (content)
                assemble_machine_code(argv[i], content);
            else
                printf("Failed to read the file %s\n", str_cat_copy(argv[i], ".am"));
        }
        else
            printf("Failed to read the file %s\n", str_cat_copy(argv[i], ".as"));
    }
}


int main(int argc, char** argv)
{
    process_args(&argc, argv);
    return 0;
}