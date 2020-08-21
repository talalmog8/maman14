#include "assembler.h"
#include <stdlib.h>

static char *name = NULL;
static char *stage = NULL;

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
 * Disposes logger if initialized
 */
void dispose_logger() {
    if (name)
        free(name);
    if (stage)
        free(stage);
}

/*
 * Logs message to stdout
 */
void log_message(char *fmt, ...) {
    va_list args;
    printf("%s: [%s] ", name, stage);
    va_start(args, fmt);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}