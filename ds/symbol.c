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

int list_exists(const symbol * ptr, char * value) {
    while(ptr != NULL)
    {
        if (!strcmp(ptr->value, value))
            return 1;

        ptr=ptr->next;
    }
    return 0;
}

/**
 * A function in charge of appending unique symbols to the symbols linked list.
 * @param head_ref A reference to the head of the symbols linked list.
 * @param new_symbol The value of the new symbol.
 * @param new_address The address of the new symbol.
 * @param new_kind The kind of the new symbol.
 * @param errors_array An array of issues in case we detect one.
 * @param ec The counter for the errors array.
 */
int append_unique(symbol** head_ref, char* new_symbol, unsigned int new_address, enum symbol_type new_kind,
        issue **errors_array, int * ec, int lc) {
    // Checking if the symbol already exists in the list:
    if (list_exists(*head_ref, new_symbol)) {
        add_new_issue_to_arr(errors_array, ec, lc, "can't define the same symbol twice.");
        return 0;
    }
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
void printList(struct Node *node)
{
    printf("    symbol_type |||| value |||| kind    \n");
    while (node != NULL)
    {
        printf("    %s |||| %u |||| %d    ", node->value, node->address, node->kind);
        node = node->next;
    }
}