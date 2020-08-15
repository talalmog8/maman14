#include "assembler.h"

#define EXTERNAL_POSTFIX ".ext"

void printexternals(char *filename, external_node *head){
    FILE *externals = NULL;

    while (head){
        externals = lazyopen(externals, filename, EXTERNAL_POSTFIX);
        fprintf(externals, "%s %07d\n", head -> label, head -> location);

        head = head -> next;
    }

    if(externals){
        fclose(externals);
    }
}