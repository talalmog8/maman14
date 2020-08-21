#include "assembler.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LABEL_SIZE 31
#define RESERVED_OPS_AMOUNT 11

static void copy(char *dest, char *source, int length);

/*
    Allocates memory for label with the specified length
*/
char *allocate_label(int length) {
    char *label;

    if (!(label = (char *) calloc(length + 1, sizeof(char)))) {
        fprintf(stderr, "Failed to allocate label. exiting program\n");
        exit(1);
    }

    return label;
}

/*
    Tries to find label in specified string. 
    If a label is found, it's length is returned. 
    Otherwise -1 is returned.
*/
int findlable(char *line, bool atStart) {
    int i = 0;

    while (line[i] != NEWLINE && line[i] != STRING_END && line[i] != COLON && !is_space(line[i]) ) {
        i++;
    }

    if (atStart) {
        if (line[i] == COLON) {
            return i;
        }
        return -1;
    }
    skip_white_characters(&line);

    if (line[i] == '\n' || line[i] == STRING_END)
        return i;
    return -1;
}

/*
    Parses a label in provided string of specified length. 
    If a valid label is found, it is copied to string that output points to, and TRUE is returned. 
    Otherwise, FALSE is returned.
*/
bool parselable(char *line, int length, char *output) {
    if (islable(line, length)) {
        copy(output, line, length);
        return TRUE;
    }

    return FALSE;
}

/*
    Validates if a line starts in a valid label of size length
    Returns TRUE if finds a valid label. Otherwise, returns FALSE
*/
bool islable(char *line, int length) {
    int i;

    struct {
        char *name;
    } reserved[] = {
            {"r1"},
            {"r2"},
            {"r3"},
            {"r4"},
            {"r5"},
            {"r6"},
            {"r7"},
            {".data"},
            {".string"},
            {".entry"},
            {".extern"},
    };

    if (length > MAX_LABEL_SIZE) {
        log_message("Label bigger than max size. Max Size: %d", MAX_LABEL_SIZE);
        return FALSE;
    }
    if (!isalpha(line[0])) {
        log_message("Label not starting with a letter");
        return FALSE;
    }


    for (i = 0; i < RESERVED_OPS_AMOUNT; ++i) {
        if (!strncmp(line, reserved[i].name, length)) {
            log_message("Reserved assembly word used as label. Reserved Name: %s", reserved[i].name);
            return FALSE;
        }
    }


    for (i = 1; i < length; i++) {
        if (!isalnum(line[i])) {
            /* label must consist of alpha numeric characters*/
            return FALSE;
        }
    }

    return TRUE;
}

/*
    Copies source string to destination string. the number of copied characters is specified.
    Adds STRING_END at the end of the copied string
*/
static void copy(char *dest, char *source, int length) {
    int i;
    for (i = 0; i < length; ++i) {
        dest[i] = source[i];
    }
    dest[length] = '\0';
}

