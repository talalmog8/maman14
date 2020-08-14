#include "assembler.h"
#include<stdlib.h>
#include<string.h>
#define POSTFIX ".as"
#define OBJECT_POSTFIX ".ob"

/*
 * TODO check warnings in functions
 */

/*
    Tries to open file with specified name + ".as" postfix
*/
FILE *openfile_for_read(char *name)
{
    char filename[MAX_FILE_NAME_LENGTH];     
    char *p= filename;

    p = strcpy(filename, name);
    p = strcat(filename, POSTFIX);

    printf("Attempting To Open File. Filename: \"%s\"\n", p);
    return fopen(p, "r");
}

/*
    Tries to open file with specified name + ".ob" postfix
*/
FILE *openfile_for_write(char *name)
{
    FILE* output;
    char filename[MAX_FILE_NAME_LENGTH];     
    char *p= filename;

    p = strcpy(filename, name);
    p = strcat(filename, OBJECT_POSTFIX);

    printf("Attempting To Open File. Filename: \"%s\"\n", p);

    if(!(output = fopen(p, "w"))){
        fprintf(stderr, "Failed to open output file. exiting program\n");
        exit(1);
    }

    return output;
}


/*
    Disposes file
*/
void disposefile(FILE *file){
    fclose(file);    
}

/*
    Reads until '\n' in specified file using dynamic memory allocation.
    If allocation fails, program exits with error code 1.
    Line length is At most 80 character without '\n'
*/
char *readline(FILE *file, char *line){
    return fgets(line, MAX_LINE_LENGTH, file);
}


