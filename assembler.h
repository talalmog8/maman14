#include<stdio.h>

#include "lableslist.h"
#include "command_template.h"

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

typedef enum {
    __string,
    __data,
    __extern,
    __entry
} guide_names;

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
bool firstpass(FILE *file);
int findlable(char * line);
bool parselable(char *line, int length, char *output);
char * allocate_label(int length);
parsing_result parse_command(char *line);


operation_names isoperation(char *text);
int is_guide(char *line);
int skip_white_characters(char *text);
bool is_comment_or_empty(char *line);

/* registers operations */
void setIC(int ic);
unsigned int getIC();
void incIC(unsigned  int addition);
void setDC(int dc);
unsigned int getDC();
void incDC(unsigned  int addition);
