#include<string.h>

typedef enum{
    label_entry = 1,
    label_external = 2
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

void printlist();
void addtoend(char *content, int location, lable_area area, lable_kind kind);
void disposelist();
stringnode *createnode(char *content, int location, lable_area area, lable_kind kind);
int exists(char *lable);