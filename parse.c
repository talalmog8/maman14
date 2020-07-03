#include "assembler.h"
#define COMMENT ';'
#define TAB '\t'
#define SPACE ' '
#define NEWLINE '\n'

static parsing_result is_comment_or_empty(char *line);

static parsing_result is_comment_or_empty(char *line){
    parsing_result result = OTHER;
    
    if (*line == NEWLINE ||*line == COMMENT)
    {
        result = CORRECT;
    }
    else if (*line == TAB || *line == SPACE)
    {
        while (*(++line) == TAB || *line == SPACE)
            ;

        if (*line == NEWLINE)
        {
            result = CORRECT;
        }
        else{
            result = OTHER;
        }
    }

    return result;
}

bool parse(FILE *file)
{
    char *line;
    int counter = 0;
    parsing_result result;

    while ((line = readline(file)) != NULL)
    {
        printf("%d: %s", ++counter, line);
        if ((result = is_comment_or_empty(line)) == CORRECT)
        {
            printf("Passing to next command due to empty line or comment\n");
        }
        else if(result == OTHER)
        {

        }        
        
        disposeline(line);
    }

    printf("\n");
    return TRUE;
}
