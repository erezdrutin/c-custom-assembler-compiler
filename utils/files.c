#include "files.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Checks if a file exists or not.
 * @param filename - The name of the file that we want to check if it exists.
 * @return 1 if the file exists, 0 if it isn't.
 */
int does_file_exist(const char *filename)
{
    FILE *file;
    if ((file = fopen(filename, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}


/**
 * A function in charge of reading a file into a dynamically allocated string.
 * @param filename The name of the file which we would like to read.
 */
char* read_file(char* filename) {
    FILE *fptr;
    long str_length;
    char *str;

    // If the file doesn't exist - returning NULL:
    if (!does_file_exist(filename)) {
        return NULL;
    }

    // Get the size of the string which will hold the file's contents:
    fptr = fopen(filename, "rb");
    fseek(fptr, 0, SEEK_END);
    str_length = ftell(fptr);
    rewind(fptr);

    // Allocate memory to contain the whole file including the '\0':
    str = (char*)malloc(str_length+1 * (sizeof(char)));

    // Read the contents of the file into the buffer:
    fread(str, 1, str_length, fptr);

    // Null to terminate the string.
    str[str_length] = '\0';

    // Close the file.
    fclose(fptr);

    // Return the contents of the file.
    return str;
}



int write_file(char* filename, char* content) {
    FILE *fp;
    // Open file in write mode
    fp = fopen(filename,"w+");

    // If file opened successfully, then write the string to file
    if (fp)
    {
        fputs(content,fp);
    }
    else
    {
        printf("Failed to open the file\n");
        return 0;
    }
    // Close the file
    fclose(fp);
    return 1;
}

int write_file_special(char *filename, word *arr, unsigned long start, unsigned long len) {
    FILE *fp = NULL;
    char *content = NULL;
    int lc = 0;

    // Open file in write mode
    unsigned long i;
    for (i = start; i < len; i++) {
        if (content == NULL)
            content = (char *)malloc((OB_FILE_ROW_LEN + 1) * sizeof(char));
        else
            content = (char *)realloc(content, (strlen(content) + OB_FILE_ROW_LEN + 1) * sizeof(char));
        snprintf(content + (lc++ * (OB_FILE_ROW_LEN + 1)), OB_FILE_ROW_LEN + 2, "%s\t%s\n", convert_to_custom_base_2_bit(arr[i].address),
                 convert_to_custom_base_2_bit(convert_10_bit_bin_to_decimal(arr[i].value)));
    }
    // Removing extra '\n' and replacing it with '\0':
    content = (char *)realloc(content, (strlen(content)) * sizeof(char));
    *(content + strlen(content) - 1) = '\0';
    write_file(filename, content);
    return 1;
}