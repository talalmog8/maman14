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

labelnode* iterate_labels(void);
void printlist();
void addtoend(char *content, int location, label_area area, label_kind kind);
void disposelist();
int exists(char *label);
void increment_data_labels(int ic);
int add_label_if_new(char *label, int location, int area, int kind);
int update_label_kind(char  *label, label_kind new_kind);
labelnode* get_label(char *label);