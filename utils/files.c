#include "files.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Checks if a file exists or not.
 * @param fileName The name of the file that we want to check if it exists.
 * @return 1 if the file exists, 0 if it isn't.
 */
int does_file_exist(const char *fileName)
{
    FILE *file;
    if ((file = fopen(fileName, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}


/**
 * A function in charge of reading a file into a dynamically allocated string.
 * @param fileName The name of the file which we would like to read.
 */
char* read_file(char* fileName) {
    FILE *fptr;
    long str_length;
    char *str;

    /* If the file doesn't exist - returning NULL: */
    if (!does_file_exist(fileName)) {
        return NULL;
    }

    /* Get the size of the string which will hold the file's contents: */
    fptr = fopen(fileName, "rb");
    fseek(fptr, 0, SEEK_END);
    str_length = ftell(fptr);
    rewind(fptr);

    /* Allocate memory to contain the whole file including the '\0': */
    str = (char*)malloc(str_length+1 * (sizeof(char)));

    /* Read the contents of the file into the buffer: */
    fread(str, 1, str_length, fptr);

    /* Null to terminate the string. */
    str[str_length] = '\0';

    /* Close the file. */
    fclose(fptr);

    /* Return the contents of the file. */
    return str;
}


/**
 * A function in charge of writing the received content to a file.
 * @param fileName The name of the file that we will write to.
 * @param content The content of the file that we would like to write to.
 * @return True (1) if we successfully wrote to the file or False (0) if we failed to write to the file.
 */
int write_file(char* fileName, char* content) {
    FILE *fp;
    /* Open file in write mode */
    fp = fopen(fileName,"w+");

    /* If file opened successfully, then write the string to the file */
    if (fp)
    {
        fputs(content,fp);
    }
    else
    {
        printf("Failed to open the file\n");
        return 0;
    }
    /* Close the file */
    fclose(fp);
    return 1;
}

/**
 * A function in charge of creating a file from the received words array.
 * The file name will be the same as the received fileName string.
 * @param fileName The name of the file that we will create.
 * @param arr An array filled with words.
 * @param start An index pointing towards the start of the array.
 * @param len The length of the array (The index of the last slot in the array).
 * @return True (1) if we successfully created the file or False (0) if we failed to create the file.
 */
int write_file_custom_word_arr(char *fileName, word *arr, unsigned long start, unsigned long len) {
    char *content = NULL;
    int lc = 0;
    unsigned long i;
    /* Loop to create a multiline string with a custom format based on the received word arr: */
    for (i = start; i < len; i++) {
        if (content == NULL)
            content = (char *)malloc((OB_FILE_ROW_LEN + 1) * sizeof(char));
        else
            content = (char *)realloc(content, (strlen(content) + OB_FILE_ROW_LEN + 1) * sizeof(char));
        snprintf(content + (lc++ * (OB_FILE_ROW_LEN + 1)), OB_FILE_ROW_LEN + 2, "%s\t%s\n", convert_to_custom_base_2_bit(arr[i].address),
                 convert_to_custom_base_2_bit(convert_10_bit_bin_to_decimal(arr[i].value)));
    }

    /* Removing extra '\n' and replacing it with '\0': */
    content = (char *)realloc(content, (strlen(content)) * sizeof(char));
    *(content + strlen(content) - 1) = '\0';

    /* Create the file: */
    return write_file(fileName, content);
}

/**
 * A function in charge of creating a file from the received symbols linked list.
 * The file name will be the same as the received fileName string.
 * @param fileName The name of the file that we will create.
 * @param ptr A pointer to a linked list.
 * @return True (1) if we successfully created the file or False (0) if we failed to create the file.
 */
int write_file_custom_symbols_ll(char *fileName, symbol *ptr) {
    char *content = NULL;
    size_t lineLen, totalLen = 0;
    /* Loop to create a multiline string with a custom format based on the received symbols ll: */
    while (ptr != NULL) {
        lineLen = strlen(ptr->value) + CUSTOM_FILE_ADD_LEN + 1;
        content = content == NULL ? (char *)malloc((lineLen) * sizeof(char)) : (char *)realloc(content, (lineLen) * sizeof(char));
        snprintf(content + (totalLen), lineLen + 1, "%s\t%s\n", ptr->value, convert_to_custom_base_2_bit(ptr->address));
        totalLen += lineLen;
        ptr=ptr->next;
    }

    /* Removing extra '\n' and replacing it with '\0': */
    *(content + totalLen - 1) = '\0';

    /* Create the file: */
    return write_file(fileName, content);
}