#include "assembler.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LABLE_SIZE 31
#define RESERVED_OPS_AMOUNT 11

char * allocate_label(int length){
    char *label;

    label = (char *)malloc((sizeof(char *) * length));

    if(!label){
        fprintf(stderr, "Failed to allocate label. exiting program\n");
        exit(1);
    }

    return label;
}

int findlable(char *line, bool atStart) {
    int i = 0;

    while (line[i] != '\n' && line[i] != '\0' && line[i] != ':') {
        i++;
    }

    if(atStart){
        if (line[i] == ':') {
            return i;
        }
        return -1;
    }
    line += skip_white_characters(line);
    if(line[i] == '\n' || line[i] == '\0')
        return i;
    return -1;
}

bool parselable(char *line, int length, char *output) {
    int i;

    if (length > MAX_LABLE_SIZE) {
        fprintf(stderr, "Lable bigger than max size\n");
        return FALSE;
    }
    if (!isalpha(line[0])) {
        fprintf(stderr, "Lable not starting with letter\n");
        return FALSE;
    }

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

    for (i = 0; i < RESERVED_OPS_AMOUNT; ++i) {
        if (!strncmp(line, reserved[i].name, length)) {
            fprintf(stderr, "reserved assembly word used as label %s\n", reserved[i].name);
            return FALSE;
        }
    }


    for(i = 1; i < length; i++){
        if(!isalnum(line[i])){
            return FALSE;
        }
    }

    output = strncpy(output, line, length);
    return TRUE;
}

