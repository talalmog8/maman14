#include "assembler.h"
#include<string.h>
#define COMMENT ';'
#define NEWLINE '\n'


#define OPS 16

int skip_white_characters(char *text){
    int i = 0;

    while (text[i] == '\t' || text[i] == ' ')
    {
        i++;
    }

    return i;
}

bool is_comment_or_empty(char *line)
{
    if (*line == NEWLINE || *line == COMMENT)
    {
        printf("EMPTYLINE \\ COMMENT\n");
        return  TRUE;
    }
    return FALSE;
}


/*
    Returns operation number identifier or -1 if text didn't match any known operation
*/
operation_names isoperation(char *text){
    int i = 0;
    operations ops[] = {
        { unknown, "unknown"},
        { mov, "mov "},
        { cmp, "cmp "},
        { add, "add "},
        { sub, "sub "},
        { lea, "lea "},
        { clr, "clr "},
        { not, "not "},
        { inc, "inc "},
        { dec, "dec "},
        { jmp, "jmp "},
        { bne, "bne "},
        { jsr, "jsr "},
        { red, "red "},     
        { prn, "prn "},
        { rts, "rts "},
        { stop, "stop "}
    };    
    
    for (; i < OPS; i++)
    {
        if(!strncmp(text, ops[i].opname, strlen(ops[i].opname))){
            return ops[i].opkind;
        }
    }
    
    return ops[0].opkind;
}

