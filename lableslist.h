#include<string.h>

typedef enum{
    entry,
    external
} lable_kind;

typedef enum{
    code,
    data
} lable_area;

typedef struct node {
    char *lable;
    int location;
    lable_kind kind;
    lable_area area; 
    struct node *next;
} stringnode;

void printlist(stringnode *head);
void addtoend(stringnode *head, char *content, int location, lable_area area, lable_kind kind);
void disposelist(stringnode *head);
stringnode *createnode(char *content, int location, lable_area area, lable_kind kind);
int exists(stringnode *head, char *lable);