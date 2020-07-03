#include "assembler.h"
#include<stdlib.h>
#include<string.h>
#define POSTFIX ".as"
#define MAX_LINE_LENGTH 81 
#define MAX_FILE_NAME_LENGTH 100

/*
    Tries to open file with specified name + ".as" postfix
*/
FILE *openfile(char *name)
{
    char filename[MAX_FILE_NAME_LENGTH];     
    char *p= filename;

    p = strcpy(filename, name);
    p = strcat(filename, POSTFIX);

    printf("Attempting To Open File. Filename: \"%s\"\n", p);
    return fopen(p, "r");
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
char* readline(FILE *file){    
    char* line = (char *)malloc(sizeof(char));
    char* start = line;
    int size = 1;
    int current;
    
    if(!line){
        fprintf(stderr, "Failed to allocate space for input lines. exiting program");
            exit(1);
    }

    while ((size <= MAX_LINE_LENGTH) && (current = fgetc(file)) != EOF)
    {
        *line = current;
        if(current == '\n'){                        
            break;   
        }                    

        line++;
        if(!realloc(start, ++size)){
            fprintf(stderr, "Failed to reallocate space for input lines. exiting program");
            exit(1);
        }
    }            

    return start;
}

/*
    Dispose line created with dynamic memory allocation
*/
void disposeline(char *linestart){
    free(linestart);
}
