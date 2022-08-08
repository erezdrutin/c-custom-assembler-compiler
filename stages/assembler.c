#include "assembler.h"
#include "first_run.h"
#include "second_run.h"
#include "../utils/string_utils.h"
#include <string.h>
#include <ctype.h>


/**
 * A function in charge of freeing the memory used in order to store the different ll versions throughout the
 * assembler process.
 * @param symbols_table_head A pointer to the head of the symbols ll (generated after the 2nd run).
 * @param ptr A pointer to the head of the original symbols ll (generated during the 1st run)
 * @param ent_table_head A pointer to the head of the entries ll.
 * @param ext_table_head A pointer to the head of the externs ll.
 */
void free_mem(symbol *symbols_table_head, symbol *ptr, symbol *ent_table_head, symbol *ext_table_head) {
    free_list(ptr);
    free_list(symbols_table_head);
    free_list(ent_table_head);
    free_list(ext_table_head);
}

/**
 * A function in charge of assembling machine code from the received content.
 * Assuming that the received content is after handling all macros (during the pre-assembly stage).
 * @param fileName The name of the file that we will create.
 * @param content The content of the ".am" file generated in the pre-assembly stage.
 */
void assemble_machine_code(char *fileName, char *content) {
    issue *errors_array = NULL;
    unsigned long pc = 100, dc = 0, mc = 100;
    symbol *symbols_table_head = NULL, *ptr = NULL, *ent_table_head = NULL, *ext_table_head = NULL;
    word *data_arr = NULL, *code_arr = NULL, *mem_arr = NULL;
    /* Errors counter */
    int ec = 0;

    /* Perform first run & update addresses based on instructions counter & data counter: */
    first_run(content, &symbols_table_head, &ent_table_head, &pc, &code_arr, &dc, &data_arr, &errors_array, &ec);
    if (!code_arr && !data_arr && ec != 0) {
        print_issues(fileName, errors_array, ec);
        free_mem(symbols_table_head, ptr, ent_table_head, ext_table_head);
        return;
    };
    mem_arr = append_word_arr(&code_arr, pc, &data_arr, dc);
    update_list_addresses(symbols_table_head, pc, symbol_data);
    update_list_addresses(ent_table_head, pc, symbol_data);

    /* Perform 2nd run & update ptr to store the head of the symbols ll before updating it: */
    second_run(content, &symbols_table_head, &ent_table_head, &mem_arr, &mc, &errors_array, &ec);
    ptr = symbols_table_head;
    symbols_table_head = generate_symbols_table(symbols_table_head, ent_table_head);

    /* Exit if we found any errors: */
    if (ec != 0) {
        print_issues(fileName, errors_array, ec);
        free_mem(symbols_table_head, ptr, ent_table_head, ext_table_head);
        return;
    }

    /* Create matching ent, ext & ob files */
    write_file_custom_symbols_ll(str_cat_copy(fileName, ".ent"), get_entry_symbols_from_ll(symbols_table_head));
    write_file_custom_symbols_ll(str_cat_copy(fileName, ".ext"), get_extern_symbols_from_ll(symbols_table_head));
    write_file_custom_word_arr(str_cat_copy(fileName, ".ob"), mem_arr, 100, pc + dc);


    /* Free memory used in the process */
    free_mem(symbols_table_head, ptr, ent_table_head, ext_table_head);
}