#include "assembler.h"

#define COMMENT ';'
#define NEWLINE '\n'
#define TAB '\t'
#define STRING_END '\0'
#define SPACE ' '

/*
 * Skips white characters in the string that the specified pointer points to
 */
void skip_white_characters(char **text_p) {
    while (**text_p == TAB || **text_p == SPACE) {
        (*text_p)++;
    }
}

/*
 * Skips the specified amount of characters in the string that the specified pointer points to.
 * Then skips white characters in the string that the specified pointer points to.
 */
void skip_characters(char **text_p, int amount){
    (*text_p) += amount;
    skip_white_characters(text_p);
}

/*
 * Returns TRUE if specified pointer points to COMMENT or NEWLINE, Otherwise, returns FALSE
 */
bool is_comment_or_empty(char *line) {
    if (*line == NEWLINE || *line == COMMENT) {
        return TRUE;
    }
    return FALSE;
}

/*
 * Returns TRUE if specified pointer points to TAB or SPACE, Otherwise, returns FALSE
 */
bool is_space(char x) {
    return (x == TAB) || (x == SPACE);
}

/*
 * Returns TRUE if specified pointer points to NEWLINE or STRING_END, Otherwise, returns FALSE
 */
bool is_end(char x) {
    return ((x == NEWLINE) || (x == STRING_END));
}
