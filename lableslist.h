#include<string.h>

typedef enum{
    label_no_kind,
    label_entry,
    label_external
} label_kind;

typedef enum{
    label_code,
    label_data
} label_area;

typedef struct node {
    char *lable;
    int location;
    label_kind kind;
    label_area area;
    struct node *next;
} labelnode;

void printlist();
void addtoend(char *content, int location, label_area area, label_kind kind);
void disposelist();
labelnode *createnode(char *content, int location, label_area area, label_kind kind);
int exists(char *lable);