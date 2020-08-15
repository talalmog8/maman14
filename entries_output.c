#include "assembler.h"

#define ENTRY_POSTFIX ".ent"

/*
 * Tries to find entries in symbol table. If it finds entries, it creates entries file.
 */
void printentries(char *filename, labelnode *head) {
    FILE *entries = NULL;

    while (head != NULL) {
        if (head->kind == label_entry) {
            entries = lazyopen(entries, filename, ENTRY_POSTFIX);
            fprintf(entries, "%s %07d\n", head->label, head->location);
        }
        head = head->next;
    }

    if (entries) {
        fclose(entries);
    }
}

