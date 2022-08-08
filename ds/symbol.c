#include "symbol.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "issue.h"
#include "../utils/validators.h"

enum search_type { includeType, excludeType };

/**
 * A function in charge of searching the list for an element with the received value.
 * @param ptr A pointer to symbols ll.
 * @param value A value to search for (symbol name).
 * @return A pointer to a symbol that answers the requirements (or NULL if there are none).
 */
symbol * search_list(symbol * ptr, char * value) {
    while(ptr != NULL)
    {
        if (!strcmp(ptr->value, value))
            return ptr;

        ptr = ptr->next;
    }
    return NULL;
}

/**
 * Searching the list for a certain type.
 * If the search type is includeType, we will search for elements that include the received type.
 * If the search type is excludeType, we will search for elements that doesn't have the received type.
 * @param ptr A pointer to symbols ll.
 * @param value A value to search for (symbol name).
 * @param st The symbol type that we would like to include / exclude in our search.
 * @param searchType The type of the search (include / exclude).
 * @return A pointer to a symbol that answers the requirements (or NULL if there are none).
 */
symbol *search_list_type(symbol *ptr, char *value, enum symbol_type st, enum search_type searchType) {
    while(ptr != NULL)
    {
        if (!strcmp(ptr->value, value) && (searchType == includeType && ptr->kind == st || searchType == excludeType && ptr->kind != st))
            return ptr;

        ptr = ptr->next;
    }
    return NULL;
}


/**
 * A function in charge of searching for duplicate occurrences of symbols in the symbols ll.
 * This function will be used combined with other validations in other to ensure that the ll contains unique symbols.
 * @param ptr A pointer to symbols ll.
 * @param value A value to search for (symbol name).
 * @param address An address that we would like the found element to include.
 * @param st The symbol type that we would like to include / exclude in our search.
 * @return A pointer to a symbol that answers the requirements (or NULL if there are none).
 */
symbol * search_list_for_dup(symbol * ptr, char * value, unsigned int address, enum symbol_type st) {
    while(ptr != NULL)
    {
        if (!strcmp(ptr->value, value) && ptr->address == address && ptr->kind == st)
            return ptr;

        ptr = ptr->next;
    }
    return NULL;
}

/**
 * A function in charge of appending errors with symbols to the errors array.
 * @param new_symbol A symbol to include in the errors array.
 * @param errors_array An array of errors that will be outputted to the user if there are any errors with the "src code".
 * @param ec Errors counter, index in the errors array.
 * @param lc Line counter, line index in the "src code".
 * @return Returns 0 (resembles that an error occurred).
 */
int append_unique_symbol_error(char* new_symbol, issue **errors_array, int *ec, int lc) {
    /* -1 for the %s part which will be replaced in the process: */
    size_t errLen = strlen("can't define the same symbol twice - %s.") + strlen(new_symbol) - 1;
    char *error = (char *) malloc((strlen("can't define the same symbol twice - %s.") +
                                   strlen(new_symbol)) * sizeof(char));
    snprintf(error, errLen, "can't define the same symbol twice - %s.", new_symbol);
    add_new_issue_to_arr(errors_array, ec, lc, error);
    return 0;
}

/**
 * A function in charge of appending nodes to the received ll.
 * The functions appends the symbol at the END of the existing ll.
 * @param head_ref A reference to the head of a symbols ll.
 * @param new_symbol A new symbol to append to either only the symbols ll or both the symbols ll & the entries ll.
 * @param new_address A new address for the new symbol.
 * @param new_kind The kind of the new symbol.
 */
void append(symbol** head_ref, char* new_symbol, unsigned int new_address, enum symbol_type new_kind)
{
    /* Allocate new symbol & last symbol pointers */
    symbol *new_node = (symbol*) malloc(sizeof(symbol));
    symbol *last = *head_ref;

    /* Initialize new symbol's attributes */
    new_node->value = strdup(new_symbol);
    new_node->address  = new_address;
    new_node->kind = new_kind;
    new_node->next = NULL;

    /* If the Linked List is empty, then make the new symbol as head */
    if (*head_ref == NULL)
    {
        *head_ref = new_node;
        return;
    }

    /* Else traverse till the last symbol */
    while (last->next != NULL)
        last = last->next;

    /* Change the next of last symbol to the newly created symbol */
    last->next = new_node;
}

/**
 * A function in charge of appending unique symbols from operator related commands (op commands).
 * @param head_ref A reference to the head of a symbols ll.
 * @param ent_table_head A reference to the head of the entries symbols ll.
 * @param new_symbol A new symbol to append to either only the symbols ll or both the symbols ll & the entries ll.
 * @param new_address A new address for the new symbol.
 * @param new_kind The kind of the new symbol.
 * @param errors_array An array of errors that will be outputted to the user if there are any errors with the "src code".
 * @param ec Errors counter, index in the errors array.
 * @param lc Line counter, line index in the "src code".
 */
void append_unique_symbol_from_op(symbol** head_ref, symbol** ent_table_head, char* new_symbol, unsigned int new_address,
                                 enum symbol_type new_kind, issue **errors_array, int *ec, int lc) {
    symbol *temp = search_list(*head_ref, new_symbol);

    /* If we're attempting to append a non-entry/non-extern symbol that already exists or an entry symbol that
     * already exists - detect an error:  */
    if ((temp != NULL && temp->kind != symbol_entry && temp->kind != symbol_extern && new_kind != symbol_entry && new_kind != symbol_extern)
        || search_list_type(*ent_table_head, new_symbol, symbol_entry, excludeType) != NULL) {
        append_unique_symbol_error(new_symbol, errors_array, ec, lc);
    }

    /* Otherwise - appending it: */
    append(head_ref, new_symbol, new_address, new_kind);
}

/**
 * A function in charge of appending unique symbols to the symbols linked list.
 * @param head_ref A reference to the head of a symbols ll.
 * @param ent_table_head A reference to the head of the entries symbols ll.
 * @param new_symbol A new symbol to append to either only the symbols ll or both the symbols ll & the entries ll.
 * @param new_address A new address for the new symbol.
 * @param new_kind The kind of the new symbol.
 * @param errors_array An array of errors that will be outputted to the user if there are any errors with the "src code".
 * @param ec Errors counter, index in the errors array.
 * @param lc Line counter, line index in the "src code".
 */
int append_unique(symbol** head_ref, symbol** ent_table_head, char* new_symbol, unsigned int new_address, enum symbol_type new_kind,
                  issue **errors_array, int * ec, int lc) {
    /* search for existing: */
    symbol *temp = search_list(*head_ref, new_symbol), *temp2 = NULL;
    temp2 = search_list(*ent_table_head, new_symbol);

    /* If we're attempting to append a non-entry/non-extern symbol that already exists OR an entry symbol that
     * already exists OR a symbol code that is defined twice - detect an error: */
    if ((temp != NULL && temp->kind != symbol_entry && temp->kind != symbol_extern && new_kind != symbol_entry && new_kind != symbol_extern)
        || search_list_type(*ent_table_head, new_symbol, symbol_entry, excludeType) != NULL
        || ((temp != NULL && new_kind == symbol_code) && (search_list_type(*head_ref, new_symbol, symbol_code, includeType) != NULL))) {
        return append_unique_symbol_error(new_symbol, errors_array, ec, lc);
    }
    /* If we detected an entry symbol - append it to both symbols ll & entries ll: */
    else if (temp2 == NULL && new_kind == symbol_entry) {
        if (temp != NULL)
            append(ent_table_head, temp->value, temp->address, temp->kind);
        append(ent_table_head, new_symbol, new_address, new_kind);
    }
    else if (temp2 != NULL && temp2->kind == symbol_entry) {
        append(ent_table_head, new_symbol, new_address, new_kind);

    }

    /* Otherwise - appending it: */
    append(head_ref, new_symbol, new_address, new_kind);
    return 1;
}

/**
 * A function in charge of updating the addresses of the received list by appending the value n to each address
 * associated with a symbol from the type st.
 * @param node A pointer to the head of the ll.
 * @param n The amount that we would like to append to st symbols' addresses.
 * @param st The type of symbols to which we would like to append the value n.
 */
void update_list_addresses(const symbol * node, unsigned int n, enum symbol_type st) {
    symbol *ptr = (symbol*)node;

    while(ptr != NULL)
    {
        if (ptr->kind == st) {
            ptr->address += n;
        }
        ptr = ptr->next;
    }
}

/**
 * A function in charge of fetching the entry symbols from the ll pointed by node.
 * @param node The head of the symbols ll.
 * @return A pointer to a new list of entry symbols.
 */
symbol *get_entry_symbols_from_ll(const symbol *node) {
    symbol *head = (symbol *)node, *temp = head, *temp1;
    symbol *res = NULL;
    while (temp != NULL) {
        temp1 = head;
        if (temp->kind == symbol_entry) {
            while (temp1 != NULL) {
                if (!strcmp(temp->value, temp1->value) && search_list_for_dup(res, temp1->value, temp1->address, temp1->kind) == NULL) {
                    append(&res, temp1->value, temp1->address, temp1->kind);
                }
                temp1=temp1->next;
            }
        }
        temp = temp->next;
    }
    return res;
}

/**
 * A function in charge of fetching the extern symbols from the ll pointed by node.
 * @param node The head of the symbols ll.
 * @return A pointer to a new list of extern symbols.
 */
symbol *get_extern_symbols_from_ll(const symbol *node) {
    symbol *head = (symbol *)node, *temp = head, *temp1;
    symbol *res = NULL;
    while (temp != NULL) {
        temp1 = head;
        if (temp->kind == symbol_extern) {
            while (temp1 != NULL) {
                if (!strcmp(temp->value, temp1->value) && temp1->kind != symbol_extern && search_list_for_dup(res, temp1->value, temp1->address, temp1->kind) == NULL) {
                    append(&res, temp1->value, temp1->address, temp1->kind);
                }
                temp1=temp1->next;
            }
        }
        temp = temp->next;
    }
    return res;
}

/**
 * A function in charge of freeing the memory used to store a linked list.
 * @param head A pointer to the head of the list that we would like to free.
 */
void free_list(symbol *head) {
    symbol * temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/**
 * A function in charge of generating a symbols table from the 2 received ll.
 * @param head The head of the existing symbols ll.
 * @param ent_table_head The head of the entries ll.
 * @return A pointer to a new "merged" ll.
 */
symbol *generate_symbols_table(const symbol *head, const symbol *ent_table_head) {
    symbol *ptr = (symbol *)head, *entPtr = (symbol *)ent_table_head, *res = NULL, *temp = NULL;
    while (ptr != NULL) {
        temp = search_list_type(entPtr, ptr->value, symbol_entry, excludeType);
        if (ptr->kind == symbol_entry && temp != NULL) {
            temp = search_list_type(entPtr, ptr->value, symbol_entry, excludeType);
            append(&res, temp->value, temp->address, symbol_entry);
        } else if (search_list(entPtr, ptr->value) == NULL) {
            append(&res, ptr->value, ptr->address, ptr->kind);
        }
        ptr = ptr->next;
    }
    return res;
}

/**
 * A function in charge of determining the symbol type of the received string.
 * @param str A string which we would like to determine its type.
 * @return The type of the symbol (data / code / invalid).
 */
enum symbol_type get_symbol_type(const char *str) {
    char *ptr = (char *)str;
    enum data_type curDataType = parse_str(ptr);
    /* Returning symbol_data if .data / .string / .struct in the string, or symbol_code if any op in string: */
    if (curDataType == sdata || curDataType == sstring || curDataType == sstruct) return symbol_data;
    else if (curDataType == op) return symbol_code;
    else if (curDataType == iextern) return symbol_extern;
    else if (curDataType == ientry) return symbol_entry;
    return symbol_invalid;
}