#include "assembler.h"
#include<stdlib.h>
#include<string.h>
#define POSTFIX ".as"
#define MAX_LINE_LENGTH 81 
#define MAX_FILE_NAME_LENGTH 100

FILE * readfile(char *name);
char* readline(FILE *file);

FILE* readfile(char *name){    
    char filename[MAX_FILE_NAME_LENGTH];     
    char *p= filename;

    p = strcpy(filename, name);
    p = strcat(filename, POSTFIX);

    return fopen(p, "r");
}

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
