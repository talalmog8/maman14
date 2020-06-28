
typedef struct node {
    char *string;
    struct node *next;
} stringnode;

void printlist(stringnode *head);
void addtoend(stringnode *head, char *content);
stringnode* createnode(char *content);
