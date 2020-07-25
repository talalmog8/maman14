#include <string.h>
#include <stdlib.h>
#include "assembler.h"
#include<ctype.h>

static bool parse_data(char *line);
static bool parse_number(char *text);

int is_guide(char *line){
    int i;

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


    for (i = 0; i < sizeof(types) / sizeof(guide_type); ++i) {
        if(!strncmp(line, types[i].type, types[i].length)){
            strcpy(line, (line + types[i].length));
            return types[i].no;
        }
    }


    return -1;
}

bool parse_guide(char *line, guide_names guide_type){
    if(guide_type == data){
        return parse_data(line);
    }

    return TRUE;
}

/*
 * Need to take care of number validation
 */
static bool parse_data(char *line){
    char *token;
    int counter = 0;

    token = strtok(line, ", \t");

    while (token != NULL){
        if(parse_number(token)){
            counter++;
        }

        token = strtok(NULL, ", \t\n\0");
    }

    incDC(counter);
    return counter > 0;
}

static bool parse_number(char *text){
    guide_template template;
    int i = 0;
    int num = 0;
    int sign = 1;

    if(*text == '-'){
        sign = -1;
        i++;
    }
    else if(*text == '+'){
        i++;
    }

    for (; text[i] != '\0'; ++i) {
        if(isdigit(text[i])){
            num *= 10;
            num += (text[i] - '0');
        }
        else{
            return FALSE;
        }
    }

    num = num * sign;

    template.data = num;
    append_guide(template);

    return TRUE;
}