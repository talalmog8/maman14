#include "assembler.h"
#include<string.h>

#define OPS 16
int stringlen(char *string);
operation_names isoperation(char *text);
int skip_white_characters(char *text);

int skip_white_characters(char *text){
    int i = 0;

    while (text[i] == '\t' || text[i] == ' ')
    {
        i++;
    }

    return i;
}

int stringlen(char *string){
    int counter = 1;
    while (*string != '\0')            
        ++counter;
    return counter;
}

/*
    Returns operation number identifier or -1 if text didn't match any known operation
*/
operation_names isoperation(char *text){
    int i = 0;
    operations ops[] = {
        { unknown, "unknown"},
        { mov, "mov"},
        { cmp, "cmp"},
        { add, "add"},
        { sub, "sub"},
        { lea, "lea"},
        { clr, "clr"},
        { not, "not"},
        { inc, "inc"},
        { dec, "dec"},
        { jmp, "jmp"},
        { bne, "bne"},
        { jsr, "jsr"},
        { red, "red"},
        { prn, "prn"},
        { rts, "rts"},
        { stop, "stop"}
    };    
    
    for (; i < OPS; i++)
    {
        if(!strncmp(text, ops[i].opname, strlen(ops[i].opname))){
            return ops[i].opkind;
        }
    }
    
    return ops[0].opkind;
}

