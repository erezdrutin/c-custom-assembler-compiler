#include "utils/files.h"
#include "utils/strings.h"
#include "utils/conversion.h"
#include <stdio.h>
#include <string.h>


int validate_file()
{
    return 0;
}




void process_args(const int* argc, char** argv) {
    int i;
    char *filename = NULL;
    char *p = NULL;
    for (i=0; i<*argc; i++) {
        // + 4 to include ".as" + '/0':
        filename = str_cat_copy(argv[i], ".as");
        printf("%s\n", filename);
//        filename = strdup(argv[i]);
//        filename = realloc(filename, )
//        filename = (char *)malloc((strlen(argv[i]) + 4)*sizeof(char));
//        strcpy(filename, argv[i]);
//        printf("%s\n", strcat(filename, ".as"));
    }
}


int main(int argc, char** argv)
{
    process_args(&argc, argv);
    printf("%s", read_file(strcat(argv[2], ".as")));
//    temp();
    printf("%s", symbols);
    return 0;
}