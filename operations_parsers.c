#include <stdlib.h>
#include "assembler.h"

static void fill_zero_args_command(int opcode, int funct);
static void dispose_operands(arguments args);

int parse_mov(char *text, int opcode, int funct) {
    arguments args;
    args = readArgs(text);
    dispose_operands(args);
}

int parse_cmp(char *text, int opcode, int funct) {
    arguments args;
    args = readArgs(text);
    dispose_operands(args);
}

int parse_add(char *text, int opcode, int funct) {
    arguments args;
    args = readArgs(text);
    dispose_operands(args);
}

int parse_sub(char *text, int opcode, int funct) {
    arguments args;
    args = readArgs(text);
    dispose_operands(args);
}

int parse_lea(char *text, int opcode, int funct) {
    arguments args;
    args = readArgs(text);
    dispose_operands(args);
}

int parse_clr(char *text, int opcode, int funct) {
    int _register;
    command_template *command;
    char *arg = read_arg(text);
   
    if(arg){            
        command = get_current_command();
        command->opcode = opcode;
        command->func = funct;
        command->orig_register = 0;
        command->orig_delivery_type = 0;
        command->A = 1;
        command->R = 0;
        command->E = 0;
        if(_register = isregister(arg)){
            command->des_register = _register;
            command->des_delivery_type = 3;    
            incIC(1);
        }        
        else if(islable(arg, strlen(arg))){
            command -> des_register = 0;
            command-> des_delivery_type = 1;
            incIC(2);
        }
        free(arg);
    }
}

int parse_not(char *text, int opcode, int funct) {
    char *arg = read_arg(text);

    if(arg){

        free(arg);
    }
}

int parse_inc(char *text, int opcode, int funct) {
    char *arg = read_arg(text);

    if(arg){

        free(arg);
    }
}

int parse_dec(char *text, int opcode, int funct) {
    char *arg = read_arg(text);

    if(arg){

        free(arg);
    }
}

int parse_jmp(char *text, int opcode, int funct) {
    char *arg = read_arg(text);

    if(arg){

        free(arg);
    }
}

int parse_bne(char *text, int opcode, int funct) {
    char *arg = read_arg(text);

    if(arg){

        free(arg);
    }
}

int parse_jsr(char *text, int opcode, int funct) {
    char *arg = read_arg(text);

    if(arg){

        free(arg);
    }
}

int parse_red(char *text, int opcode, int funct) {
    char *arg = read_arg(text);

    if(arg){

        free(arg);
    }
}

int parse_prn(char *text, int opcode, int funct) {
    char *arg = read_arg(text);

    if(arg){

        free(arg);
    }
}

int parse_rts(char *text, int opcode, int funct) {
    fill_zero_args_command(opcode, funct);
    return TRUE;
}

int parse_stop(char *text, int opcode, int funct) {
    fill_zero_args_command(opcode, funct);
    return TRUE;
}

static void dispose_operands(arguments args){
    if(args.arg1 != NULL && args.arg2 != NULL){

        free(args.arg1);
        free(args.arg2);
    }
    else if(args.arg1 != NULL){
        free(args.arg1);
    }
    else if(args.arg2 != NULL){
        free(args.arg2);
    }
}

static void fill_zero_args_command(int opcode, int funct){
    command_template *command;
    command = get_current_command();
    command->opcode = opcode;
    command->func = funct;
    command->orig_register = 0;
    command->orig_delivery_type = 0;
    command->des_register = 0;
    command->des_delivery_type = 0;
    command->A = 1;
    command->R = 0;
    command->E = 0;
    incIC(1);
}