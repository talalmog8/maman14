#include "assembler.h"
#include <stdlib.h>

static command_template  *commands_p;
static int current_command;

static guide_template *guides_p;
static int current_guide;


void assign_output_arrays(void){
    commands_p = malloc(sizeof(command_template) * MAX_PROGRAM_OUTPUT_SIZE);
    if(!commands_p){
        printf("Failed to allocate memory to command array");
        exit(1);
    }

    guides_p = malloc(sizeof(guide_template) * MAX_PROGRAM_OUTPUT_SIZE);

    if(!guides_p){
        printf("Failed to allocate memory to guide array");
        exit(1);
    }

    reset_output_arrays();
}

void reset_output_arrays(void){
    current_guide = 0;
    current_command = 0;
}

void append_command(command_template command){
    commands_p[current_command++] = command;
}

void append_guide(guide_template guide){
    guides_p[current_guide++] = guide;
}

void dispose_output_arrays(void){
    free(commands_p);
    free(guides_p);
}

void print_output_arrays(void){
    int i;
    printf("commands:\n");
    for (i = 0; i < current_command; ++i) {
        printf("no: %d %06X\n", i, commands_p[i]);
    }
    printf("guides:\n");
    for (i = 0; i < current_guide; ++i) {
        printf("no: %d %06X\n", i, guides_p[i]);
    }
}

