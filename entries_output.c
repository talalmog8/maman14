#include <stdlib.h>
#include "assembler.h"

#define ENTRY_POSTFIX ".ent"


static FILE *lazyopen(FILE *file, char *filename);

/*
 * Tries to find entries in symbol table. If it finds entries, it creates entries file.
 */
void printentries(char *filename, labelnode *head) {
    FILE *entries = NULL;

    while (head != NULL) {
        if (head->kind == label_entry) {
            entries = lazyopen(entries, filename);
            fprintf(entries, "%s %07d\n", head->lable, head->location);
        }
        head = head->next;
    }

    if (entries) {
        fclose(entries);
    }
}

/*
 * Opens file if not open yet, for write operations with 'ent' postfix
 */
static FILE *lazyopen(FILE *file, char *filename){
    if (!file) {
        /* lazy initialization */
        file = openfile_for_write(filename, ENTRY_POSTFIX);
        if (!file) {
            fprintf(stderr, "Failed to open file for entries. filename: %s\n", filename);
            exit(1);
        }
    }
    return file;
}
