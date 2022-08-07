#include "string_utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/**
 * A function in charge of copying, concatenating and allocating memory for a string combined of 2 strings.
 * For example, if we were to pass str1 = "test", str2 = " hello", the function would:
 * 1. Allocate enough memory to store the concatenated string.
 * 2. Copy both strings one after the other.
 * 3. Return a pointer to the new "merged" string.
 * @param str1 - First string in the result.
 * @param str2 - Second string in the result.
 * @return A new string combined from both received strings.
 */
char* str_cat_copy(const char *str1, const char *str2) {
    unsigned long str1_len, str2_len;
    char *new_str;

    /* If we received 2 empty strings - return null: */
    if (str1 == NULL && str2 == NULL)
        return NULL;

    str1_len = str1 == NULL ? 0 : strlen(str1);
    str2_len = str2 == NULL ? 0 : strlen(str2);

    new_str = malloc(str1_len + str2_len + 1);

    /* If we fail to allocate memory for some reason - return null: */
    if (new_str == NULL)
        return NULL;

    memcpy(new_str, str1, str1_len);
    memcpy(new_str + str1_len, str2, str2_len + 1);

    return new_str;
}

/**
 * Search and replace all occurrences of a substring (token) by another str (replaceWith) in the receive str (str).
 * Note: The function handles any relevant memory allocations.
 * @param str The string we would like to change.
 * @param token The token which we would like to replace.
 * @param replaceWith The string we would like to replace the token with.
 */
void str_replace(char *str, const char *token, const char *replaceWith){
    /* As long as we find occurrences of the token: */
    while (strstr(str, token) != NULL) {
        char *temp = malloc(strlen(strstr(str, token) + strlen(token)) + 1);
        /* Create a str with what's after the replaced part */
        strcpy(temp, strstr(str, token) + strlen(token));
        /* Take away the part to replace and the part after it in the initial str */
        *strstr(str, token) = '\0';
        /* Concat the first part of the str with the part to replace with */
        strcat(str, replaceWith);
        /* Concat the first part of the str with the part after the replaced part */
        strcat(str, temp);
        /* Free the memory to avoid memory leaks */
        free(temp);
    }
}

/**
 * A function in charge of counting how many characters appear between a certain character and a token (substring).
 * Suppose we wanted to count how many characters are between b and hello in: "83434mb wqe hello hello",
 * we would use this function and it will return 5 (the amount of chars between the first occurrence of b and hello).
 * @param string The string in which we would like to perform the search.
 * @param token The token that resembles the "end" of our count.
 * @param ch The character that resembles the "beginning" of our count.
 * @return A size_t value that resembles the amount of chars between the received char & the token.
 */
size_t calc_chars_between_ch_and_token(const char* string, const char* token, char ch) {
    size_t j, sum;
    if (!strstr(string, token)){
        return 0;
    }
    sum = (size_t) ((size_t)strlen(string) - (size_t)strlen((char *)strstr(string, token)));
    for(j = sum; j >= 0; j--) {
        if (string[j] == ch) {
            // Returning the amount of chars until reaching the ch:
            return sum - (j + 1);
        }
    }
    return sum;
}

/**
 * A function in charge of creating a string of certain length that only contains the received character.
 * @param ch A character to create a string of n*char.
 * @param length The length of the string containing the same char n times.
 * @return An allocated string for the received char & length.
 */
char * create_chars_str(char ch, size_t length) {
    size_t i;
    char* str = calloc(length, 1);
    for (i=0; i<length-1; i++){
        str[i] = ch;
    }
    str[length] = '\0';
    return str;
}

/**
 * A function in charge of removing spaces from a string.
 * @param s The string from which we would like to remove spaces.
 */
void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ' || *d == '\n' || *d == '\t') {
            ++d;
        }
    } while ((*s++ = *d++));
}

/**
 * Trim whitespaces from the left side.
 * @param s A string to remove whitespaces from.
 * @return The string after removing the whitespaces from it.
 */
char* ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

/**
 * Trim whitespaces from the right side.
 * @param s A string to remove whitespaces from.
 * @return The string after removing the whitespaces from it.
 */
char* rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

/**
 * A function that trims a string from whitespaces that appear in it.
 * @param s A string to remove whitespaces from.
 * @return A string without whitespaces.
 */
char* trim(char *s)
{
    return rtrim(ltrim(s));
}

/**
 * A function in charge of trimming whitespaces from a multiline string.
 * @param s A string that contains content (which may be multiline).
 * @return A string without whitespaces.
 */
char* trim_multiline_string(const char* s) {
    const char * curLine = s;
    char* newStr = NULL, *tempStr = NULL;
    size_t curLineLen;
    while(curLine)
    {
        // Fetch next line and perform relevant allocations:
        const char * nextLine = strchr(curLine, '\n');
        curLineLen = nextLine ? (nextLine-curLine) : strlen(curLine);
        tempStr = (char *) malloc(curLineLen+1);
        if (tempStr)
        {
            memcpy(tempStr, curLine, curLineLen);
            tempStr[curLineLen] = '\0';  // NUL-terminate!
            tempStr = trim(tempStr);
            newStr = str_cat_copy(newStr, tempStr);
            newStr[strlen(newStr)] = '\n';
        }
        else printf("malloc() failed!?\n");
        curLine = nextLine ? (nextLine+1) : NULL;
    }

    if (newStr) {
        newStr[strlen(newStr)-1] = '\0';
    }
    return newStr;
}

/**
 * A function in charge of adding spaces to the left of each row in a (possibly) multiline string.
 * @param s A string that contains content (which may be multiline).
 * @param spaces_count The amount of spaces to add to each row from the left side.
 * @return A string with left spaces appended to each of its rows.
 */
char* lspaces_append(const char* s, size_t spaces_count) {
    const char * curLine = s;
    char* newStr = NULL, *tempStr = NULL;
    size_t curLineLen;
    while(curLine)
    {
        // Fetch next line and perform relevant allocations:
        const char * nextLine = strchr(curLine, '\n');
        curLineLen = nextLine ? (nextLine-curLine) : strlen(curLine);
        tempStr = (char *) malloc(curLineLen + 1);
        if (tempStr && spaces_count)
        {
            // Increasing by 1 to include '\0' if we received a valid count:
            char* spaces = create_chars_str(' ', spaces_count + 1);
            memcpy(tempStr, curLine, curLineLen);
            tempStr[curLineLen] = '\0';  // NUL-terminate!
            tempStr = str_cat_copy(spaces, tempStr);
            newStr = str_cat_copy(newStr, tempStr);
            newStr[strlen(newStr)] = '\n';
        }
        else if (!tempStr) printf("malloc() failed\n");
        curLine = nextLine ? (nextLine+1) : NULL;
    }

    if (newStr) {
        newStr[strlen(newStr)-1] = '\0';
        return newStr;
    } else {
        return (char *)s;
    }
}

/**
 * Returns if a string is empty or not.
 * @param s A string.
 * @return True (1) if the string is empty / False (0) if the string isn't empty.
 */
int is_empty(const char *s) {
    while (*s != '\0') {
        if (!isspace((unsigned char)*s))
            return 0;
        s++;
    }
    return 1;
}

/**
 * A function in charge of counting how many times a certain character appears in a string.
 * @param str A string to check.
 * @param ch A character to search for.
 * @return The amount of times ch appears in str.
 */
size_t count_char_in_string(const char *str, char ch) {
    size_t i;
    char *ptr = (char *)str;
    for (i = 0; ptr[i]; ptr[i] == ch ? i++ : *ptr++);
    return i;
}

/**
 * A function in charge of determining whether a given string is a number or not.
 * @param s A string.
 * @return True (1) or False (0).
 */
int is_number(char *s)
{
    int i;
    for (i = 0; s[i]!= '\0'; i++)
    {
        if (!(isdigit(s[i]) || ((s[i] == '-' || s[i]=='+') && isdigit(s[i+1]))))
            return 0;
    }
    return 1;
}

/**
 * A function in charge of counting how many times a number appears in a string.
 * Assuming that the numbers are separated by the received sep parameter.
 * @param str A string to search for numbers in.
 * @param sep A separator that the numbers are assumed to be separated by.
 * @return An integer representing how many times a number appeared in the string.
 */
int count_numbers_in_string(const char *str) {
    int count = 0;
    char *p = (char *)str;
    while (*p) { // While there are more characters to process...
        if ( isdigit(*p) || ( (*p=='-'||*p=='+') && isdigit(*(p+1)) )) {
            // Found a number
            long val = strtol(p, &p, 10); // Read number
            count++;
        } else {
            // Otherwise, move on to the next character.
            p++;
        }
    }
    return count;
}
