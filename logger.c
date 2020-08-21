#include "assembler.h"
#include <stdlib.h>

static char *name = NULL; /* logger name - will set to current .as file's name */
static char *stage = NULL; /* logger stage - will set to firstpass/secondpass */
static char *faulted_line = NULL; /* logger faulted line - will be set to current line being parsed */
static unsigned int faulted_line_number; /* logger faulted line number - will be set to current line number being parsed */

/*
 * Initializes logger with file's name and stage (firstpass/secondpass)
 */
void set_logger(char *filename, char *log_stage) {
    dispose_logger();

    if (!(name = calloc(1, strlen(filename) + 1))) {
        fprintf(stderr, "Failed to allocate memory for logger. Exiting program");
        exit(1);
    }
    if (!(stage = calloc(1, strlen(log_stage) + 1))) {
        fprintf(stderr, "Failed to allocate memory for logger. Exiting program");
        exit(1);
    }

    strcpy(name, filename);
    strcat(name, ".as");
    strcpy(stage, log_stage);
}

/*
 * Allocates memory, and saves current line. save current line number as well.
 * This information will be printed in logs.
 */
void set_logger_current_line(char *current_line, unsigned int line_no) {
    int line_length;

    if (faulted_line) {
        free(faulted_line);
    }

    /* allocate enough memory for current line with line's delimiter */
    if (!(faulted_line = calloc(1, (line_length = strlen(current_line))))) {
        fprintf(stderr, "Failed to allocate memory for logger. Exiting program");
        exit(1);
    }

    strncpy(faulted_line, current_line, line_length - 1); /* copy faulted line without \n or \0 */
    faulted_line[line_length-1] = '\0';
    faulted_line_number = line_no;
}

/*
 * Disposes allocated memory for source file current line
 */
void dispose_logger_current_line(void){
    if(faulted_line){
        free(faulted_line);
        faulted_line = NULL;
        faulted_line_number = 0;
    }
}

/*
 * Disposes logger if initialized
 */
void dispose_logger(void) {
    if (name)
        free(name);
    if (stage)
        free(stage);
    if (faulted_line)
        free(faulted_line);
}

/*
 * Logs message to stdout
 */
void log_message(char *fmt, ...) {
    va_list args;
    printf("%s: [%s] ", name, stage);
    va_start(args, fmt);
    vprintf(fmt, args);
    if (faulted_line) {
        printf(". Error in line: %d. Content: [%s]", faulted_line_number, faulted_line);
    }
    printf("\n");
    va_end(args);
}