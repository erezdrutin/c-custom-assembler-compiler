#include "symbol.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "issue.h"
#include <stdlib.h>

/* Given a reference (pointer to pointer) to the head of a list
   and an int,  inserts a new symbol on the front of the list. */
void push(struct Node** head_ref, char* new_symbol, unsigned int new_address, enum symbol_type new_kind)
{
    /* 1. allocate symbol */
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));

    /* 2. put in the value  */
    new_node->value = new_symbol;
    new_node->address  = new_address;
    new_node->kind = new_kind;

    /* 3. Make next of new symbol as head */
    new_node->next = (*head_ref);

    /* 4. move the head to point to the new symbol */
    (*head_ref) = new_node;
}

/* Given a symbol prev_node, insert a new symbol after the given
prev_node */
void insertAfter(struct Node* prev_node, char* new_symbol, unsigned int new_address, enum symbol_type new_kind)
{
    /*1. check if the given prev_node is NULL */
    if (prev_node == NULL) {
        printf("the given previous symbol cannot be NULL");
        return;
    }

    /* 2. allocate new symbol */
    struct Node* new_node
            = (struct Node*)malloc(sizeof(struct Node));

    /* 3. put in the value */
    new_node->value = new_symbol;
    new_node->address = new_address;
    new_node->kind = new_kind;

    /* 4. Make next of new symbol as next of prev_node */
    new_node->next = prev_node->next;

    /* 5. move the next of prev_node as new_node */
    prev_node->next = new_node;
}

symbol * search_list(symbol * ptr, char * value) {
    while(ptr != NULL)
    {
        if (!strcmp(ptr->value, value))
            return ptr;

        ptr = ptr->next;
    }
    return NULL;
}

symbol * search_list_with_type(symbol * ptr, char * value, enum symbol_type st) {
    while(ptr != NULL)
    {
        if (!strcmp(ptr->value, value) && ptr->kind == st)
            return ptr;

        ptr = ptr->next;
    }
    return NULL;
}

/**
 * Search for all the symbols that share the same name as the received value whilst not having the received type.
 * @param ptr A pointer to the head of the ll.
 * @param value The value that we would like to search for.
 * @param st The type that we wouldn't like to include in the search.
 * @return A pointer to a matching node / NULL if none were found.
 */
symbol *search_list_without_type(symbol * ptr, char * value, enum symbol_type st) {
    while(ptr != NULL)
    {
        if (!strcmp(ptr->value, value) && ptr->kind != st)
            return ptr;

        ptr = ptr->next;
    }
    return NULL;
}


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
 * NEWNEWNEWNEW
 * @param ptr
 * @param st
 * @return
 */
symbol * search_list_for_type(symbol * ptr, enum symbol_type st) {
    symbol *newHead = NULL;
    while(ptr != NULL)
    {
        if (ptr->kind == st)
            append(&newHead, ptr->value, ptr->address, ptr->kind);

        ptr = ptr->next;
    }
    return newHead;
}


int list_exists(const symbol * ptr, char * value) {
    while(ptr != NULL)
    {
        if (!strcmp(ptr->value, value))
            return 1;

        ptr=ptr->next;
    }
    return 0;
}

int list_exists_with_type(const symbol *ptr, char *value, enum symbol_type st) {
    while(ptr != NULL)
    {
        if (!strcmp(ptr->value, value) && ptr->kind == st)
            return 1;

        ptr=ptr->next;
    }
    return 0;
}

/**
 * A function in charge of appending unique symbols to the symbols linked list.
 * @param head_ref A reference to the head of the symbols linked list.
 * @param ent_table_head A reference to the head of a symbols ll for entries.
 * @param new_symbol The value of the new symbol.
 * @param new_address The address of the new symbol.
 * @param new_kind The kind of the new symbol.
 * @param errors_array An array of issues in case we detect one.
 * @param ec The counter for the errors array.
 */
int append_unique(symbol** head_ref, symbol** ent_table_head, char* new_symbol, unsigned int new_address, enum symbol_type new_kind,
        issue **errors_array, int * ec, int lc) {
    // search for existing:
    symbol *temp = search_list(*head_ref, new_symbol), *temp2 = NULL;
    // If there is an existing node that isn't entry / extern - return an error:
    if (temp != NULL && temp->kind != symbol_entry && temp->kind != symbol_extern && new_kind != symbol_entry && new_kind != symbol_extern) {
        // -1 for the %s part which will be replaced in the process:
        size_t errLen = strlen("can't define the same symbol twice - %s.") + strlen(new_symbol) - 1;
        char *error = (char *) malloc((strlen("can't define the same symbol twice - %s.") +
                                       strlen(new_symbol)) * sizeof(char));
        snprintf(error, errLen, "can't define the same symbol twice - %s.", new_symbol);
        add_new_issue_to_arr(errors_array, ec, lc, error);
        return 0;
    }

    // Entries - search for existing symbol in entries table.
    // If found only a symbol of type entry - append the new version:
    // If found anything else - there is a problem:
    temp2 = search_list(*ent_table_head, new_symbol);
    if (search_list_without_type(*ent_table_head, new_symbol, symbol_entry)) {
        // -1 for the %s part which will be replaced in the process:
        size_t errLen = strlen("can't use the same entry twice - %s.") + strlen(new_symbol) - 1;
        char *error = (char *) malloc((strlen("can't use the same entry twice - %s.") +
                                       strlen(new_symbol)) * sizeof(char));
        snprintf(error, errLen, "can't use the same entry twice - %s.", new_symbol);
        add_new_issue_to_arr(errors_array, ec, lc, error);
        return 0;
    }
    else if (temp2 == NULL && new_kind == symbol_entry) {
        if (temp != NULL)
            append(ent_table_head, temp->value, temp->address, temp->kind);
        append(ent_table_head, new_symbol, new_address, new_kind);
    }
    else if (temp2 != NULL && temp2->kind == symbol_entry) {
        append(ent_table_head, new_symbol, new_address, new_kind);

    }
//    else if ((temp == NULL && new_kind == symbol_entry) || (temp != NULL && temp->kind == symbol_entry)) {
//        append(ent_table_head, new_symbol, new_address, new_kind);
//    }

    // Otherwise - appending it:
    append(head_ref, new_symbol, new_address, new_kind);
    return 1;
}

/* Given a reference (pointer to pointer) to the head
   of a list and an int, appends a new symbol at the end  */
void append(struct Node** head_ref, char* new_symbol, unsigned int new_address, enum symbol_type new_kind)
{
    /* 1. allocate symbol */
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));

    struct Node *last = *head_ref;  /* used in step 5*/

    /* 2. put in the value  */
    new_node->value = strdup(new_symbol);
    new_node->address  = new_address;
    new_node->kind = new_kind;

    /* 3. This new symbol is going to be the last symbol, so make next
          of it as NULL*/
    new_node->next = NULL;

    /* 4. If the Linked List is empty, then make the new symbol as head */
    if (*head_ref == NULL)
    {
        *head_ref = new_node;
        return;
    }

    /* 5. Else traverse till the last symbol */
    while (last->next != NULL)
        last = last->next;

    /* 6. Change the next of last symbol */
    last->next = new_node;
}

// This function prints contents of linked list starting from head
void printList(const struct Node *node)
{
    struct Node *temp = (struct Node*)node;
    printf("    symbol_type |||| value |||| kind    \n");
    while (temp != NULL)
    {
        printf("    %s |||| %u |||| %d    ", temp->value, temp->address, temp->kind);
        temp = temp->next;
    }
}

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
 * Normally I wouldn't go with such method, but since we're talking a linked list whose size is limited (due to the
 * 256 memory addresses limitation), I felt like O(n^2) is good enough and is simpler to maintain rather than working
 * with Hashtables or other DS.
 * Anyways, the point of this function is to update the address of each entry or extern symbol based
 * @param node
 * @param st
 */
void update_list_custom_type(const symbol * node, enum symbol_type st) {
    symbol *head = (symbol *)node, *temp = head, *temp1;

    while(temp != NULL)
    {
        temp1 = head;
        if (temp->kind == st) {
            while(temp1 != NULL) {
                if (!strcmp(temp->value, temp1->value) && temp->address != temp1->address) {
                    temp->address = temp1->address;
                }
                temp1=temp1->next;
            }
        }
        temp=temp->next;
    }
}

symbol * get_entry_symbols_from_ll(const symbol *node) {
    symbol *head = (symbol *)node, *temp = head, *temp1;
    symbol *res = NULL;
    while (temp != NULL) {
        temp1 = head;
        if (temp->kind == symbol_entry) {
            while (temp1 != NULL) {
                //temp1->kind != symbol_entry &&
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

symbol * get_extern_symbols_from_ll(const symbol *node) {
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

symbol *generate_symbols_table(const symbol *head, const symbol *ent_table_head) {
    symbol *ptr = (symbol *)head, *entPtr = (symbol *)ent_table_head, *res = NULL, *temp = NULL;
    while (ptr != NULL) {
        if (ptr->kind == symbol_entry && search_list(entPtr, ptr->value) != NULL) {
            temp = search_list_without_type(entPtr, ptr->value, symbol_entry);
            append(&res, temp->value, temp->address, symbol_entry);
        } else if (search_list(entPtr, ptr->value) == NULL) {
            append(&res, ptr->value, ptr->address, ptr->kind);
        }
        ptr = ptr->next;
    }
    return res;
}