#include "assembler.h"
#include<string.h>
#include <ctype.h>
#include <stdlib.h>

bool firstpass_parse_command(char **line_p){
    char *line = *line_p;
    operation operation;

    if((operation = isoperation(line_p)).opcode == -1){
        fprintf(stderr, "entered unknown operation in line %s", line);
        return FALSE;
    }

    operation.parser(*line_p, operation.opcode, operation.funct);
    return TRUE;
}

bool secondpass_parse_command(char **line_p){
    char *line = *line_p;
    operation operation;

    if((operation = isoperation(line_p)).opcode == -1){
        fprintf(stderr, "entered unknown operation in line %s", line);
        return FALSE;
    }

    if(operation.label_inserter){
        operation.label_inserter(*line_p);
    }
    return TRUE;
}

/*
    Returns operation number identifier or -1 if text didn't match any known operation
    Also moves text pointer after the operation name that was found
*/
operation isoperation(char **text_p){
    char *text = *text_p;
    int i = 0, length;
    operation ops[] = {
            { -1, 0, "unknown", NULL},
            { 0, 0, "mov", parse_two_args_command, secondpass_two_args_command },
            { 1, 0,"cmp",  parse_two_args_command, secondpass_two_args_command },
            { 2, 1, "add", parse_two_args_command, secondpass_two_args_command },
            { 2, 2, "sub", parse_two_args_command, secondpass_two_args_command },
            { 4, 0, "lea",   parse_two_args_command, secondpass_two_args_command },
            { 5, 1, "clr",   parse_one_arg_command, secondpass_one_arg_command },
            { 5, 2, "not",   parse_one_arg_command, secondpass_one_arg_command },
            { 5, 3, "inc",   parse_one_arg_command, secondpass_one_arg_command },
            { 5, 4, "dec",   parse_one_arg_command, secondpass_one_arg_command },
            { 9, 1, "jmp",   parse_one_arg_command, secondpass_one_arg_command},
            { 9, 2, "bne",   parse_one_arg_command, secondpass_one_arg_command},
            { 9, 3, "jsr",   parse_one_arg_command, secondpass_one_arg_command},
            { 12, 0, "red",  parse_one_arg_command, secondpass_one_arg_command},
            { 13, 0, "prn",  parse_one_arg_command, secondpass_one_arg_command},
            { 14, 0, "rts",  fill_zero_args_command, secondpass_zero_arg_command},
            { 15, 0, "stop", fill_zero_args_command, secondpass_zero_arg_command}
    };

    for (; i < (sizeof(ops) / sizeof(operation)); i++)
    {
        if((!strncmp(text, ops[i].opname, (length = strlen(ops[i].opname))))){
            if(!isspace(text[length])){
                fprintf(stderr, "Missing space or tab after operation name. Line: %s", text);
                return ops[0];
            }
            skip_characters(text_p, length);
            return ops[i];
        }
    }

    return ops[0];  
}

arguments read_args(char *line){
    arguments args;
    char *token, *arg1 = NULL, *arg2 = NULL;

    if((token = strtok(line, ", \t\n\0"))){
        arg1 = malloc(sizeof(char)*strlen(token));
        if(!arg1){
            fprintf(stderr, "Failed to allocate memory for argument. exiting...");
            exit(1);
        }
        strcpy(arg1, token);
        if((token = strtok(NULL, ", \t\n\0"))){
            arg2 = malloc(sizeof(char)*strlen(token));
            if(!arg2){
                fprintf(stderr, "Failed to allocate memory for argument. exiting...");
                exit(1);
            }
            strcpy(arg2, token);
        }
        else{
            fprintf(stderr, "Managed to read only 1 operands from 2. arguments: %s", line);
        }
    }
    else{
        fprintf(stderr, "Managed to read only 0 operands from 2. arguments: %s", line);
    }

    args.arg1 = arg1;
    args.arg2 = arg2;

    return args;
}

void dispose_operands(arguments args) {
    if (args.arg1 != NULL && args.arg2 != NULL) {
        free(args.arg1);
        free(args.arg2);
    } else if (args.arg1 != NULL) {
        free(args.arg1);
    } else if (args.arg2 != NULL) {
        free(args.arg2);
    }
}

char *read_arg(char *line){
    char *token;
    char *arg = NULL;
    token = strtok(line, " \t\n\0");

    if(token){
        if(strtok(NULL, " \t\n\0")){
            fprintf(stderr, "More arguments than expected\n");
        }
        arg  = malloc(sizeof(char) * strlen(token));
        strcpy(arg, token);
        return arg;
    }

    fprintf(stderr, "Managed to read only 0 operands from 1. arguments: %s", line);
    return  arg;
}



/*
    Checks if string is a register. If it is, it returns the register's number, otherwise it returns -1
*/
int isregister(char *arg){    
    if(*(arg++) == 'r'){
        if(*arg >= '0' && *arg <= '7'){
            return (*arg - '0');
        }
    }

    return -1;
}

int try_parse_number(char* arg, int *number){
    int i = 1;
    int num = 0;
    int sign = 1;

    if(*arg != '#'){
        return FALSE;
    }

    if(arg[i] == '-'){
        sign = -1;
        i++;
    }
    else if(arg[i] == '+'){
        i++;
    }

    for (; arg[i] != '\0'; ++i) {
        if(isdigit(arg[i])){
            num *= 10;
            num += (arg[i] - '0');
        }
        else{            
            return FALSE;
        }
    }

    *number = num * sign;     
    return TRUE;
}

int isaddress(char *arg){
    if(*arg == '&'){
        arg++; /* skip & */
        if(islable(arg, (int)strlen(arg))){
            return TRUE;            
        }
        fprintf(stderr, "Not a valid label after \'&\' sign. sign: %s", arg + 1);
        return FALSE;
    }
    return FALSE;
}

