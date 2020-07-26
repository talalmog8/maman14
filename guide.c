#include <string.h>
#include <stdlib.h>
#include "assembler.h"
#include<ctype.h>

static bool parse_numbers(char *line);
static bool parse_number(char *text);
static bool parse_string(char *text);
static bool is_space(char  x);
static bool is_end(char  x);

int is_guide(char *line){
    int i;

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
            strcpy(line, (line + types[i].length + skip_white_characters(line + types[i].length)));
            return types[i].no;
        }
    }


    return -1;
}

bool parse_guide(char *line, guide_names guide_type){
    if(guide_type == data){
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

static bool parse_string(char *text){
    int i;
    guide_template  template;
    text += skip_white_characters(text);

    if(*text != '\"'){
        fprintf(stderr, "Missing \" suffix in .string argument. argument: %s", text);
        return FALSE;
    }

    for (i = 1; text[i] != '\"' && text[i] != '\n' && text[i] != '\0' ; ++i) {
        if((text[i] > 32 && text[i] < 127) || isspace(text[i])){
            template.data = text[i];
            append_guide(template);
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
    else if(!is_end(skip_white_characters((text + i + 1)))){
        fprintf(stderr, "Excess data after .string argument. argument: %s", text);
        return FALSE;
    }

    template.data = '\0';
    append_guide(template);

    return TRUE;
}

static bool is_space(char  x){
    return (x == '\t') || (x == ' ');
}

static bool is_end(char  x){
    return ((x == '\n') || (x == '\0'));
}