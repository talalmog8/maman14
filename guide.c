#include <string.h>
#include <stdlib.h>
#include "assembler.h"
#include<ctype.h>

static bool parse_numbers(char *line);
static bool parse_number(char *text);
static bool parse_string(char *text);

int is_guide(char **line_p){
    int i;
    char *line = (*line_p);

    typedef struct{
        char *type;
        size_t length;
        guide_names no;
    } guide_type;

    guide_type  types[] = {
            {".string", strlen(".string"), __string},
            {".data", strlen(".data"), __data},
            {".entry", strlen(".entry"), __entry},
            {".extern", strlen(".extern"), __extern}
    };


    for (i = 0; i < sizeof(types) / sizeof(guide_type); ++i) {
        if(!strncmp(line, types[i].type, types[i].length) && is_space(line[types[i].length])){
            printf("Guide: %s", line);
            skip_characters(line_p, types[i].length);
            return types[i].no;
        }
    }


    return -1;
}

bool parse_guide(char *line, guide_names guide_type){
    if(guide_type == __data){
        return parse_numbers(line);
    }
    return parse_string(line);
}

/*
 * Need to take care of number validation
 */
static bool parse_numbers(char *line){
    char *token;
    int counter = 0;

    token = strtok(line, ", \t\n\0");

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
            fprintf(stderr, "Could not parse numbers in .data command. arguements: %s", text);
            return FALSE;
        }
    }

    num = num * sign;
    get_current_guide()->data = num;
    return TRUE;
}

static bool parse_string(char *text){
    int i;
    skip_white_characters(&text);

    if(*text != '\"'){
        fprintf(stderr, "Missing \" suffix in .string argument. argument: %s", text);
        return FALSE;
    }

    for (i = 1; text[i] != '\"' && text[i] != '\n' && text[i] != '\0' ; ++i) {
        if((text[i] > 32 && text[i] < 127) || isspace(text[i])){
            get_current_guide() -> data = text[i];;
            incDC(1);
        }
        else{
            fprintf(stderr, "Found illegal character in .string guide. arguments: %s", text);
            return FALSE;
        }
    }

    if(text[i] != '\"'){
        fprintf(stderr, "Missing \" postfix in .string argument. argument: %s", text);
        return FALSE;
    }

    skip_characters(&text,i + 1);
    if(!is_end(*text)){
        fprintf(stderr, "Excess data after .string argument. argument: %s", text);
        return FALSE;
    }

    get_current_guide() -> data = '\0';
    incDC(1);
    return TRUE;
}



