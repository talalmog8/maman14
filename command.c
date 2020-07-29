#include "assembler.h"
#include<string.h>
#include <ctype.h>

#define PARSER_NAME(opname) parse_##opname

bool parse_command(char **line_p){
    char *line = *line_p;
    operation operation;

    if((operation = isoperation(line_p)).opcode == -1){
        fprintf(stderr, "entered unknown operation in line %s", line);
        return FALSE;
    }

    operation.parser(*line_p, operation.opcode, operation.funct);
    printf("Command: %s", line);
    return TRUE;
}

/*
    Returns operation number identifier or -1 if text didn't match any known operation
    Also moves text pointer after the operation name that was found
*/
operation isoperation(char **text_p){
    char *text = *text_p;
    int i = 0, length;
    operation ops[] = {
            { -1, 0, "unknown", NULL},
            { 0, 0, "mov", PARSER_NAME(mov)},
            { 1, 0,"cmp", PARSER_NAME(cmp)},
            { 2, 1, "add", PARSER_NAME(add)},
            { 2, 2, "sub", PARSER_NAME(sub)},
            { 4, 0, "lea", PARSER_NAME(lea)},
            { 5, 1, "clr", PARSER_NAME(clr)},
            { 5, 2, "not", PARSER_NAME(not)},
            { 5, 3, "inc", PARSER_NAME(inc)},
            { 5, 4, "dec", PARSER_NAME(dec)},
            { 9, 1, "jmp", PARSER_NAME(jmp)},
            { 9, 2, "bne", PARSER_NAME(bne)},
            { 9, 3, "jsr", PARSER_NAME(jsr)},
            { 12, 0, "red", PARSER_NAME(red)},
            { 13, 0, "prn", PARSER_NAME(prn)},
            { 14, 0, "rts", PARSER_NAME(rts)},
            { 15, 0, "stop", PARSER_NAME(stop)}
    };

    for (; i < (sizeof(ops) / sizeof(operation)); i++)
    {
        if((!strncmp(text, ops[i].opname, (length = strlen(ops[i].opname))))){
            if(!isspace(text[length])){
                fprintf(stderr, "Missing space or tab after operation name. Line: %s", text);
                return ops[0];
            }
            skip_characters(text_p, length);
            return ops[i];
        }
    }

    return ops[0];
}




