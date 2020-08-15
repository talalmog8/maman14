#include "assembler.h"
#include <stdlib.h>

static external_node *createnode(char *label, int location);
static char *allocate_external(char *from);

static  external_node *head = NULL;

external_node * iterate_externals(void){
    return head;
}

void add_external(char *label, int location)
{
    external_node *current = head;

    if (head == NULL) {
        head = createnode(label, location);
        return;
    }

    while ((current->next) != NULL)
        current = current->next;

    current->next = createnode(label, location);
}

static external_node *createnode(char *label, int location)
{
    external_node *new = (external_node *)malloc(sizeof(external_node));

    if (!new)
    {
        fprintf(stderr, "couldn't allocate memory for linked list node\n");
        exit(1);
    }

    new -> label = allocate_external(label);
    new -> location = location;
    new -> next = NULL;

    return new;
}

static char *allocate_external(char *from){
    char *to;

    to = (char *)calloc(strlen(from) + 1, sizeof(char));
    if(!to){
        fprintf(stderr, "Failed to allocate external. exiting program\n");
        exit(1);
    }

    strcpy(to, from);

    return to;
}

void dispose_externals()
{
    external_node *current = head, *next;

    if (current == NULL)
        return;

    while ((current->next) != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    free(current -> label);
    free(current);

    head = NULL;
}


