#include "assembler.h"

#include <string.h>
#include <ctype.h>

#define MAX_LABLE_SIZE 31
#define RESERVED_OPS_AMOUNT 11


char *findlable(char *text) {
    int i = 0;

    while (text[i] != '\n' && text[i] != ':') {
        i++;
    }

    if (text[i] == ':') {
        return (text + i + 1);
    }

    return NULL;
}

bool parselable(char *lable, int length) {
    int i;

    if (length > MAX_LABLE_SIZE) {
        fprintf(stderr, "Lable bigger than max size\n");
        return FALSE;
    }
    if (!isalpha(lable[0])) {
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
        if (!strncmp(lable, reserved[i].name, length)) {
            fprintf(stderr, "reserved assembly word used as label %s\n", reserved[i].name);
            return FALSE;
        }
    }
}

