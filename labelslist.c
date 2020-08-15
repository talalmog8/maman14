#include "assembler.h"
#include <stdlib.h>

/*
    This file holds functions to manipulate labels linked list
*/

static  labelnode* head = NULL; /* holds the head of the labels linked list */
static labelnode *createnode(char *, int , label_area, label_kind);

/*
 * Used for iterating over the labels list in different files
 */
labelnode* iterate_labels(void){
    return head;
}

/*
 * Adds a new label to the end of the labels list, with the provided information
 */
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

/*
 * Creates a new node for labels list, holding the provided information.
 * The node's memory is dynamically allocated.
 * A pointer to the node is returned.
 */
static labelnode *createnode(char *content, int location, label_area area, label_kind kind)
{
    labelnode *new = (labelnode *)malloc(sizeof(labelnode));

    if (!new)
    {
        fprintf(stderr, "couldn't allocate memory for linked list node\n");
        exit(1);
    }

    new->label = content;
    new->location = location;
    new->area = area;
    new->kind = kind;
    new -> next = NULL;
    return new;
}

/*
 * Disposes all memory that has been dynamic allocated in labels list
 */
void dispose_labels(void)
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
    free(current->label);
    free(current);

    head = NULL;
}

/*
 * Returns 1 if a label with the specified name is stored in labels list.
 * Otherwise, returns 0
 */
int exists(char *label){
    labelnode *current = head;

    while (current != NULL){
        if(!strcmp(current->label, label)){
            return 1;
        }        
        current = current -> next;
    }
    return 0;    
}

/*
 * Searches for a label with the specified name in labels list. If such label is found, a pointer to it is returned.
 * Otherwise, NULL is returned.
 */
labelnode* get_label(char *label){
    labelnode *current = head;

    while (current != NULL){
        if(!strcmp(current->label, label)){
            return current;
        }
        current = current -> next;
    }
    return NULL;
}

/*
 * Tries to find label with specified name in labels list.
 * If such label is found, the label's kind is updated using the specified label kind, and 1 is returned.
 * Otherwise, if such label isn't found, 0 is returned.
 */
int update_label_kind(char  *label, label_kind new_kind){
    labelnode *current = head;

    while (current != NULL){
        if(!strcmp(current->label, label)){
            current -> kind = new_kind;
            return 1;
        }
        current = current -> next;
    }
    return 0;
}

/*
 * Adds specified ic to all non label_external labels that classified as label_data
 */
void increment_data_labels(int ic){
    labelnode *current = head;

    while (current != NULL){
        if((current -> area == label_data) && (current -> kind != label_external) ){
            current -> location += ic;
        }
        current = current -> next;
    }
}

/*
 * Tries to add specified label to labels list.
 * If specified label already exists in list, returns false, Otherwise, adds label to end of the list, and returns TRUE
 */
int add_label_if_new(char *label, int location, int area, int kind) {
    if (exists(label)) {
        fprintf(stderr, "Label already exists. Label: %s\n", label);
        return FALSE;
    } else {
        addtoend(label, location, area, kind);
        return TRUE;
    }
}