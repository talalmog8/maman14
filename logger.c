#include "assembler.h"
#include <stdlib.h>

static char *name = NULL;
static char *stage = NULL;
static char *faulted_line = NULL;

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

void set_logger_current_line(char *current_line) {
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
}

void dispose_logger_current_line(void){
    if(faulted_line){
        free(faulted_line);
        faulted_line = NULL;
    }
}

/*
 * Disposes logger if initialized
 */
void dispose_logger() {
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
        printf(". Faulted line: [%s]", faulted_line);
    }
    printf("\n");
    va_end(args);
}