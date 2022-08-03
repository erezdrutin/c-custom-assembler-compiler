#include "macros.h"
#include "../utils/string_utils.h"
#include <stdlib.h>
#include <string.h>

/**
 * A function in charge of extracting the name of the macro that it receives a pointer to.
 * Notes:
 * This function assumes that it's being passed a pointer to the word "macro" after detecting a macro definition.
 * @param ptrStart A pointer to the word macro in it's definition.
 * @return A pointer to a string containing the name of the macro.
 */
char* extract_macro_name(const char* ptrStart) {
    char* ptrEnd = strchr(ptrStart, '\n');
    char* temp = (char *)calloc(1, ptrEnd - ptrStart);
    char* macroName = NULL;

    /* Using a helping parameter to "hold" the definition line: */
    memcpy(temp, ptrStart, ptrEnd - ptrStart);
    /* Removing all spaces without touching the received content: */
    remove_spaces(temp);
    /* Allocating enough place to hold the name of the macro: */
    macroName = (char *)calloc(1, strlen(temp) - strlen("macro"));
    /* Copying the macro name: */
    memcpy(macroName, temp + strlen("macro"), strlen(temp) - strlen("macro"));

    free(temp);
    return macroName;
}

/**
 * A function in charge of extracting a macro entire definition (returns macro x...endmacro).
 * The main purpose of this function is to act as a pre-step to removing this part from the src-code when translating
 * to pre-assembly code.
 * Notes:
 * This function assumes that it's being passed a pointer to the word "macro" after detecting a macro definition.
 * @param ptrStart A pointer to the word macro in it's definition.
 * @return A string containing the entire definition of the macro.
 */
char* extract_macro_definition(char* ptrStart) {
    char* ptrEnd = strstr(ptrStart, "endmacro") + strlen("endmacro") + 1;
    char* macroDefinition = NULL;

    if (!ptrStart || !ptrEnd)
        return NULL;

    macroDefinition = (char *)calloc(1, ptrEnd - ptrStart + 1);
    memcpy(macroDefinition, ptrStart, ptrEnd - ptrStart);

    return macroDefinition;
}

/**
 * A function in charge of extracting the "pure" content of the macro (the lines which will replace the macro name)
 * after the pre-assembly step.
 * Notes:
 * This function assumes that it's being passed a pointer to the word "macro" after detecting a macro definition.
 * @param ptr A pointer to the word macro in it's definition.
 * @return A string containing the "pure" content of the macro.
 */
char* extract_macro_content(const char* ptr) {
    char *ptrStart = strchr(ptr, '\n') + 1;
    char *ptrEnd = strstr(ptr, "endmacro") - 1;
    char *initialMacroContent = NULL, *macroContent = NULL;

    if (!ptrStart || !ptrEnd)
        return NULL;

    initialMacroContent = (char *)calloc(1, ptrEnd - ptrStart + 1);
    memcpy(initialMacroContent, ptrStart, ptrEnd - ptrStart);
    macroContent = trim_multiline_string(initialMacroContent);
    free(initialMacroContent);
    return macroContent;
}


/**
 * A function that might as well sit in the string.c file but sits here since its only related to macros.
 * @param str A string to alter.
 * @param token A token that represents a macro.
 * @param replaceWith A string to replace every occurrence of the token in str with.
 */
void str_replace_macro(char *str, const char *token, const char *replaceWith){
    /* As long as we find occurrences of the token: */
    char *temp = NULL;
    while (strstr(str, token) != NULL) {
        size_t spaces_count = calc_chars_between_ch_and_token(str, token, '\n');
        char *curReplaceWith = lspaces_append(replaceWith, spaces_count);
        temp = malloc(strlen(strstr(str, token) + strlen(token)) + 1);
        /* Create a str with what's after the replaced part */
        strcpy(temp, strstr(str, token) + strlen(token));
        /* Take away the part to replace and the part after it in the initial str */
        *(strstr(str, token) - spaces_count) = '\0';
        /* Concat the first part of the str with the part to replace with */
        strcat(str, curReplaceWith);
        /* Concat the first part of the str with the part after the replaced part */
        strcat(str, temp);
    }
    /* TODO: Free the memory to avoid memory leaks */
    free(temp);
}