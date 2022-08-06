#include <stdlib.h>

#ifndef MAMAN14_STRINGS_H
#define MAMAN14_STRINGS_H
char* str_cat_copy(const char *str1, const char *str2);
void str_replace(char *str, const char *token, const char *replaceWith);
void remove_spaces(char* s);
char* ltrim(char *s);
char* rtrim(char *s);
char* trim(char *s);
char* trim_multiline_string(const char* s);
char* lspaces_append(const char* s, size_t spaces_count);
size_t calc_chars_between_ch_and_token(const char* string, const char* token, char ch);
int is_empty(const char *s);
size_t count_char_in_string(const char *str, char ch);
char * create_chars_str(char ch, size_t length);
char *strrev(char *str);
int is_number(char *s);
int count_numbers_in_string(const char *str);
#endif //MAMAN14_STRINGS_H
