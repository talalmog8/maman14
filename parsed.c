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

command_template* get_current_command(){
    return &commands_p[current_command++];
}

command_template* get_command_by_ic(unsigned int ic){
    return &commands_p[ic - 100];
}

guide_template* get_current_guide(){
    return &guides_p[current_guide++];
}

void dispose_output_arrays(void){
    free(guides_p);
    free(commands_p);
}

void print_output_arrays(char *filename, int ic, int dc){
    command_2_integer commandconverter;
    guide_2_integer guideconverter;
    int i;
    FILE *file = openfile_for_write(filename);

    if(!file){
        fprintf(stderr, "Filed to open file for assembler object file. filename: %s\n", filename);
        exit(1);
    }

    fprintf(file,"\t%d %d\n", (ic - 100) , dc);
    for (i = 0; i < current_command; ++i) {
        commandconverter.command = commands_p[i];
        fprintf(file, "%07d %06x\n", (i + 100), commandconverter.command_as_integer);
    }    
    for (i = 0; i < current_guide; ++i) {
        guideconverter.guide = guides_p[i];
        fprintf(file, "%07d %06x\n", (i + ic), guideconverter.guide_as_integer);
    }
}

