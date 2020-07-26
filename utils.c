#include "assembler.h"

#define COMMENT ';'
#define NEWLINE '\n'


#define OPS 16

void skip_white_characters(char **text_p) {
    while (**text_p == '\t' || **text_p == ' ') {
        (*text_p)++;
    }
}

void skip_characters(char **text_p, int amount){
    (*text_p) += amount;
    skip_white_characters(text_p);
}

bool is_comment_or_empty(char *line) {
    if (*line == NEWLINE || *line == COMMENT) {
        printf("EMPTYLINE \\ COMMENT\n");
        return TRUE;
    }
    return FALSE;
}

bool is_space(char x) {
    return (x == '\t') || (x == ' ');
}

bool is_end(char x) {
    return ((x == '\n') || (x == '\0'));
}
