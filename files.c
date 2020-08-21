#include "assembler.h"
#include<stdlib.h>
#include<string.h>

#define POSTFIX ".as"

/*
    Tries to open file with specified name + ".ob" postfix
*/
FILE *openfile_for_write(char *name, char *postfix) {
    FILE *output;
    char filename[MAX_FILE_NAME_LENGTH];
    char *p;

    p = strcat(strcpy(filename, name), postfix);

    if (!(output = fopen(p, "w"))) {
        fprintf(stderr, "Failed to open output file. exiting program\n");
        exit(1);
    }

    return output;
}

/*
    Tries to open file with specified name + ".as" postfix
*/
FILE *openfile_for_read(char *name) {
    char filename[MAX_FILE_NAME_LENGTH];
    char *p;

    p = strcat(strcpy(filename, name), POSTFIX);

    return fopen(p, "r");
}


/*
    Disposes file
*/
void disposefile(FILE *file) {
    fclose(file);
}

/*
    Reads until '\n' in specified file using dynamic memory allocation.
    If allocation fails, program exits with error code 1.
    Line length is At most 80 character without '\n'
*/
char *readline(FILE *file, char *line) {
    return fgets(line, MAX_LINE_LENGTH, file);
}


/*
 * Opens file if not open yet, for write operations with 'ent' postfix
 */
FILE *lazyopen(FILE *file, char *filename, char *postfix) {
    if (!file) {
        /* lazy initialization */
        file = openfile_for_write(filename, postfix);
        if (!file) {
            fprintf(stderr, "Failed to open file for entries. filename: %s\n", filename);
            exit(1);
        }
    }
    return file;
}

