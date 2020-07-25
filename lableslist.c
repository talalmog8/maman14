#include "assembler.h"
#include <stdlib.h>

/*
    This file holds function to for linked list operation of char array nodes

*/

static  stringnode* head = NULL;

void printlist()
{
    int index = 0;
    stringnode *current = head;

    while (current != NULL)
    {
        printf("%d: lable: %s location: %d area: %s kind: %s\n", (index++), current->lable, current->location
        , (current -> area) ? "data" : "code", ((current -> kind) == 1) ? "entry" : "external");
        current = current->next;
    }
}

void addtoend(char *content, int location, lable_area area, lable_kind kind)
{
    stringnode *current = head;

    if (head == NULL) {
        head = createnode(content, location, area, kind);
        return;
    }

    while ((current->next) != NULL)
        current = current->next;

    current->next = createnode(content, location, area, kind);
}

stringnode *createnode(char *content, int location, lable_area area, lable_kind kind)
{
    stringnode *new = (stringnode *)malloc(sizeof(stringnode));

    if (!new)
    {
        fprintf(stderr, "couldn't allocate memory for linked list node\n");
        exit(1);
    }

    new->lable = content;
    new->location = location;
    new->area = area;
    new->kind = kind;
    new -> next = NULL;
    return new;
}

void disposelist()
{
    stringnode *current = head, *next;

    if (current == NULL)
        return;

    while ((current->next) != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    free(current->lable);
    free(current);
}

int exists(char *lable){
    stringnode *current = head;

    if (current == NULL)
        return 0;

    while ((current->next) != NULL){
        if(!strcmp(current->lable, lable)){
            return 1;
        }        
        current = current -> next;
    }
    return 0;    
}
