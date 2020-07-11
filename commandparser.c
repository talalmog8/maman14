#include "assembler.h"

typedef struct{
    operation_names name;
    bool (* parser)(char *);
} parseFuncs;

parsing_result main_parser(char *line){
    char *lable_result;
    parsing_result result = OTHER;
    line += skip_white_characters(line);

    if(lable_result = findlable(line)){
        if(parselable(line, (lable_result - line - 1))){
            /* add lable to list if is a valid lable */
            printf("FOUND LABLE\n");
        }
        else{
            return ERROR;
        }
    }

    if(isoperation(line) == unknown){
        fprintf(stderr, "entered unknown operation in line %s", line);
        return ERROR;
    }
    
    return result;
}

bool isguide(char *text){
    return TRUE;
}
