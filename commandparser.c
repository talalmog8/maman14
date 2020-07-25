#include <string.h>
#include "assembler.h"
#define GUIDE_AMOUNT 4
/*
parsing_result parse_command(char *line){
    char *lable_result;
    parsing_result result = OTHER;
    line += skip_white_characters(line);

    if(lable_result = findlable(line)){
        if(parselable(line, (lable_result - line - 1))){

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

*/

int is_guide(char *line){
    int i;
    line += skip_white_characters(line);

    typedef struct{
        char *type;
        size_t length;
        guide_names no;
    } guide_type;

    guide_type  types[] = {
            {".string ", strlen(".string "), __string},
            {".data ", strlen(".data "), __data},
            {".entry ", strlen(".entry "), __entry},
            {".extern ", strlen(".extern "), __extern}
    };


    for (i = 0; i < GUIDE_AMOUNT; ++i) {
        if(!strncmp(line, types[i].type, types[i].length)){
            line += types[i].length;
            return types[i].no;
        }
    }


    return -1;
}
