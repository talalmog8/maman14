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

typedef enum {
    unknown = -1,    
    mov,
    cmp,
    add,
    sub,
    lea,
    clr,
    not,
    inc,
    dec,
    jmp,
    bne,
    jsr,
    red,
    prn,
    rts,
    stop
} operation_names;

typedef struct
{
    operation_names opkind;
    char* opname;
} operations;


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
char* findlable(char * text);
bool parselable(char *lable, int length);
parsing_result main_parser(char *line);

operation_names isoperation(char *text);
bool isguide(char *text);
int skip_white_characters(char *text);