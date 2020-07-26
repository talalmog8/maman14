#include "assembler.h"
#define COMMENT ';'
#define NEWLINE '\n'


#define OPS 16

int skip_white_characters(char *text){
    int i = 0;

    while (text[i] == '\t' || text[i] == ' ')
    {
        i++;
    }

    return i;
}

bool is_comment_or_empty(char *line)
{
    if (*line == NEWLINE || *line == COMMENT)
    {
        printf("EMPTYLINE \\ COMMENT\n");
        return  TRUE;
    }
    return FALSE;
}

bool is_space(char  x){
    return (x == '\t') || (x == ' ');
}

