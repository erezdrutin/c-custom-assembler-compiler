#include "pre_assembly.h"
#include "macros.h"
#include "string_utils.h"
#include "files.h"
#include <stdlib.h>
#include <string.h>

/**
 * A function in charge of expanding the macros (if there are any) in the received source code.
 * @param content The content of the source code.
 */
void handle_macros(char *content) {
    const char* ptr = content;
    char *temp = NULL, *macroDefinition = NULL, *macroContent = NULL, *macroName = NULL;

    /* Searching for macros in the content string: */
    while ((temp = strstr(ptr, "macro"))) {
        /* Extracting the name, content & definition parts of the current macro from the content string: */
        macroName = extract_macro_name(temp);
        macroContent = extract_macro_content(temp);
        macroDefinition = extract_macro_definition(temp);

        /* Removing the macro definition: */
        str_replace(content, macroDefinition, "");
        /* Replacing each occurrence of the macro with its content: */
        str_replace_macro(content, macroName, macroContent);

        /* Freeing the memory used in the process: */
        free(macroName);
        free(macroContent);
        free(macroDefinition);
    }
}


/**
 * This function performs the entire pre_assembly stage that consists of 3 steps:
 * 1. Read the source code supplied by the user.
 * 2. Find & replace each macro call with it's contents (+ Remove macro definition).
 * 3. Create a new file with the extension ".am".
 * @param fileName The name of the file we would like to parse.
 */
void pre_assembly(char* fileName) {
    char* newFileName = strdup(fileName);
    char* content = read_file(fileName);
    /* Replace the received filename extension .as with .am: */
    str_replace(newFileName, ".as", ".am");
    /* Expand & handle macros from the source file: */
    handle_macros(content);
    /* Write a ".am" file with the source-code after handling the macros: */
    write_file(newFileName, content);
}