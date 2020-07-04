#include "assembler.h"
#define COMMENT ';'
#define TAB '\t'
#define SPACE ' '
#define NEWLINE '\n'
#define PARSERS_AMOUNT 1
#define END_OF_STRING '\0'

static parsing_result is_comment_or_empty(char *line);

static parsing_result is_comment_or_empty(char *line)
{
    parsing_result result = OTHER;

    if (*line == NEWLINE || *line == COMMENT)
    {
        result = CORRECT;
    }
    else if (*line == TAB || *line == SPACE)
    {
        while (*(++line) == TAB || *line == SPACE)
            ;

        if (*line == NEWLINE || *line == END_OF_STRING)
        {
            result = CORRECT;
        }
        else
        {
            result = OTHER;
        }
    }

    printf("Parser: is_comment_or_empty. Result: %d\n", result);
    return result;
}

bool parse(FILE *file)
{
    char *line;
    int counter = 0;
    int i;
    parsing_result result;
    parsing_result (*parsers[])(char *text) = {
        is_comment_or_empty};

    while ((line = readline(file)) != NULL)
    {        
        printf("%d: %s", ++counter, line);

        for (i = 0, result = OTHER; (i < PARSERS_AMOUNT) && (result != CORRECT); i++)
        {
            result = parsers[i](line);

            if (result == ERROR)
            {
                /* error in parse. need to add to linked list  */
            }
        }
        disposeline(line);
    }

    printf("\n");
    return TRUE;
}
