#include "assembler.h"
#include <stdlib.h>

/*
    This file holds function to for linked list operation of char array nodes

*/

static  labelnode* head = NULL;

void printlist()
{
    int index = 0;
    labelnode *current = head;
    printf("Labels:\n");
    while (current != NULL)
    {
        printf("%d: label: %s location: %d area: %d kind: %d\n", (index++), current->lable, current->location, current -> area, current -> kind);
        current = current->next;
    }
}

void addtoend(char *content, int location, label_area area, label_kind kind)
{
    labelnode *current = head;

    if (head == NULL) {
        head = createnode(content, location, area, kind);
        return;
    }

    while ((current->next) != NULL)
        current = current->next;

    current->next = createnode(content, location, area, kind);
}

labelnode *createnode(char *content, int location, label_area area, label_kind kind)
{
    labelnode *new = (labelnode *)malloc(sizeof(labelnode));

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
    labelnode *current = head, *next;

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
    labelnode *current = head;

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
