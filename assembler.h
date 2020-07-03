#include<stdio.h>

typedef enum {
    FALSE,
    TRUE    
} bool;

typedef enum {
    CORRECT,
    OTHER,
    ERROR
} parsing_result;

/* command line arguements */
char * nextArg(int amount, char* args[]);
FILE * readFile(char *name);


/* file */
FILE * openfile(char *name);
char* readline(FILE *file);
void disposefile(FILE *file);
void disposeline(char *linestart);

/* parsing */
bool parse(FILE *file);
int isLable(char * text);

