#include "assembler.h"
#include<string.h>
#include <ctype.h>

bool parse_command(char *line){
    operation_names operation;

    if((operation = isoperation(line)) == unknown){
        fprintf(stderr, "entered unknown operation in line %s", line);
        return FALSE;
    }

    printf("Command: %s", line);
    return TRUE;
}

/*
    Returns operation number identifier or -1 if text didn't match any known operation
*/
operation_names isoperation(char *text){
    int i = 0, length;
    operation ops[] = {
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

    for (; i < (sizeof(ops)/ sizeof(operation)); i++)
    {
        if((!strncmp(text, ops[i].opname, (length = strlen(ops[i].opname))))){
            if(!isspace(text[length])){
                fprintf(stderr, "Missing spece or tab after operation name. Line: %s", text);
                return unknown;
            }
            return ops[i].opkind;
        }
    }

    return unknown;
}




