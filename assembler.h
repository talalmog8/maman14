#include<stdio.h>

/* command line arguements */
char * nextArg(int amount, char* args[]);
FILE * readFile(char *name);


/* file */
FILE * openfile(char *name);
char* readline(FILE *file);
void disposefile(FILE *file);
void disposeline(char *linestart);
