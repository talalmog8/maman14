#include <stdlib.h>
#include "assembler.h"

static int parse_one_arg_command(char *text, int opcode, int funct);
static void fill_one_arg_command_defaults(command_template *command, int opcode, int funct, int des_register, int des_delivery_type);
static bool parse_zero_args_command(char *text, int opcode, int funct);
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
    int parsed, delivery_type;

    parsed =  parse_one_arg_command(text, opcode, funct);
    delivery_type = peek_last_command() -> des_delivery_type;

    if(!parsed){
        return  FALSE;
    }
    else if(delivery_type == 1 || delivery_type == 3){
        return TRUE;
    }
    else{
        fprintf(stderr, "Not a valid delivery type fo clr command. delivery type: %d", delivery_type);
    }
}

int parse_not(char *text, int opcode, int funct) {
    return parse_one_arg_command(text, opcode, funct);
}

int parse_inc(char *text, int opcode, int funct) {
    return parse_one_arg_command(text, opcode, funct);
}

int parse_dec(char *text, int opcode, int funct) {
    return parse_one_arg_command(text, opcode, funct);
}

int parse_jmp(char *text, int opcode, int funct) {
    char *arg = read_arg(text);
    command_template *current;

    if(arg){
        current = get_current_command();
        current -> opcode = opcode;
        current -> func = funct;
        current -> orig_delivery_type = 0;
        current -> orig_register = 0;

        current-> A = 1;
        current-> R = 0;
        current-> E = 0;
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
    return  parse_zero_args_command(text, opcode, funct);
}

int parse_stop(char *text, int opcode, int funct) {
    return parse_zero_args_command(text, opcode, funct);
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

static int parse_one_arg_command(char *text, int opcode, int funct) {
    int _register;
    int parsed = 0;
    command_template *command;
    char *arg = read_arg(text);

    if (arg) {
        command = get_current_command();
        if ((_register = isregister(arg)) != -1) {
            fill_one_arg_command_defaults(command, opcode, funct, _register, 3);
            parsed = 1;
        } else if (islable(arg, strlen(arg))) {
            fill_one_arg_command_defaults(command, opcode, funct, 0, 1);
            incIC(1); /* saved space for label address*/
            parsed = 1;
        }
        free(arg);
    }

    return parsed;
}

static void fill_one_arg_command_defaults(command_template *command, int opcode, int funct, int des_register, int des_delivery_type){
    command->opcode = opcode;
    command->func = funct;
    command->orig_register = 0;
    command->orig_delivery_type = 0;
    command->des_register = des_register;
    command->des_delivery_type = des_delivery_type;
    command->A = 1;
    command->R = 0;
    command->E = 0;
    incIC(1);
}

static bool parse_zero_args_command(char *text, int opcode, int funct){
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

    if(!is_end(*text)){
        fprintf(stderr, "Excess text in command.text: %s", text);
        return  FALSE;
    }
    return TRUE;
}