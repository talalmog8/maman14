#include "assembler.h"
#include <stdlib.h>
#include "lableslist.h"

/*
    This file holds function to for linked list operation of char array nodes

*/

void printlist(stringnode *head)
{
    int index = 0;
    stringnode *current = head;

    while (current != NULL)
    {
        printf("%d: lable: %s location: %d area: %s kind: %s\n", (index++), current->lable, current->location
        , (current -> area) ? "data" : "code", (current -> kind) ? "external" : "entry");
        current = current->next;
    }
}

void addtoend(stringnode *head, char *content, int location, lable_area area, lable_kind kind)
{
    stringnode *current = head;

    if (current == NULL)
        return;

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
    return new;
}

void disposelist(stringnode *head)
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
    free(current);
}

int main(void)
{
    stringnode *head = createnode("a", 100, code, external);
    addtoend(head, "a", 100, code, external);
    addtoend(head, "aa", 101, data, entry);
    addtoend(head, "aaa", 102, code, external);
    addtoend(head, "aaaa", 103, data, entry);
    addtoend(head, "aaaaa", 104, code, external);
    printlist(head);
    disposelist(head);
    return 0;
}