#include "utils/files.h"
#include "utils/conversions.h"
#include "utils/strings.h"
#include "stages/pre_assembly.h"
#include <stdio.h>
#include <string.h>
#include "stages/assembler.h"
#include "ds/operators.h"

int validate_file()
{
    return 0;
}

//char * testos(long long x) {
//    int length = snprintf( NULL, 0, "%lld", x );
//    char* str = malloc( length + 1 );
//    snprintf( str, length + 1, "%lld", x );
//    return str;
//}




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
//    char str[400] = "hello this is a test ok>";
//    process_args(&argc, argv);
//    printf("%s", read_file(strcat(argv[2], ".as")));
//    temp();
//    printf("%s\n", symbols);
//    str_replace(str, "is", "siu siu siu");
//    printf("%s", str);
    pre_assembly("macrotest.as");

//    /* Start with the empty list */
//    struct Node* head = NULL;
//
//    // Insert 6.  So linked list becomes 6->NULL
//    append(&head, "erez", 6, code);
//
//    // Insert 7 at the beginning. So linked list becomes 7->6->NULL
//    push(&head, "noice", 7, data);
//
//    // Insert 1 at the beginning. So linked list becomes 1->7->6->NULL
//    push(&head, "ayo", 1, code);
//
//    // Insert 4 at the end. So linked list becomes 1->7->6->4->NULL
//    append(&head, "waddup", 4, data);
//
//    // Insert 8, after 7. So linked list becomes 1->7->8->6->4->NULL
//    insertAfter(head->next, "siu", 8, data);
//
//    printf("\n Created Linked list is: ");
//    printList(head);

//    create_symbols_table(read_file("macrotest.am"));
//    printf("%s", convert_to_custom_base(1026));

//    printf("%s", read_file("macrotest.am"));
    printList(generate_symbols(read_file("assemblertest.am")));
//    char str[100] = "STR: .string \"abcdef\"\nABC: .string \"ghijklmnop\"\nSIU: .string \"ghijklmnop\"";
//    printList(generate_symbols(str));
//    testos();
//    validate_operator_usage("erez");

//    char str[11];
//    convert_to_10_bit_bin(1022, str);
//    printf("%s\n\n", str);
//    printf("%d", convert_10_bit_bin_to_decimal(str));
    return 0;
}