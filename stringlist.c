#include "assembler.h"
#include<stdlib.h>
#include "linkedlists.h"

/*
    This file holds function to for linked list operation of char array nodes

*/

void printlist(stringnode *head);
void addtoend(stringnode *head, char *content);
stringnode* createnode(char *content);

void printlist(stringnode *head){
    int index = 0;
    stringnode *current = head;

    while (current != NULL)
    {
        printf("%d: %s\t", (index++), current -> string);
        current = current -> next;
    }    
    printf("\n");
}

void addtoend(stringnode *head, char *content){
    stringnode *current = head;

    while ((current -> next) != NULL)
        current = current -> next;

    current -> next = createnode(content);        
}

stringnode* createnode(char *content){
    stringnode *new = (stringnode *)malloc(sizeof(stringnode));

    if(!new){
        fprintf(stderr, "couldn't allocate memory for linked list node\n");
        exit(1);
    }
    
    new -> string = content;

    return new;
}
