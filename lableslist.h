
typedef struct node {
    char *lable;
    int location;
    struct node *next;
} stringnode;

void printlist(stringnode *head);
void addtoend(stringnode *head, char *content, int location);
void disposelist(stringnode *head);
stringnode* createnode(char *content, int location);
