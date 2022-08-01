#ifndef MAMAN14_MACROS_H
#define MAMAN14_MACROS_H
char* extract_macro_name(const char* ptrStart);
char* extract_macro_definition(char* ptrStart);
char* extract_macro_content(const char* ptr);
void str_replace_macro(char *str, const char *token, const char *replaceWith);
#endif //MAMAN14_MACROS_H
