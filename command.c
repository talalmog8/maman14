#include "assembler.h"
#include<string.h>
#include <ctype.h>

#define MAX_REG 7
#define MIN_REG 0
#define REG_SUFFIX 'r'

#define NUMBER_SUFFIX '#'
#define ADDRESS_SUFFIX '&'

#define POSITIVE_NUMBER_SUFFIX '+'
#define NEGATIVE_NUMBER_SUFFIX '-'

/*
 * This file contains function for parsing commands
 */

/*
 * Checks if string contains known command. If such command is found, the proper parser function is executed and TRUE is returned,
 * Otherwise, FALSE is returned.
 */
bool firstpass_parse_command(char **line_p) {
    operation operation;

    if ((operation = isoperation(line_p)).opcode == -1) {
        return FALSE;
    }

    if (operation.parser(*line_p, operation.opcode, operation.funct, operation.id) != -1) {
        return TRUE;
    }

    return FALSE;
}

/*
 * Checks if string contains known command. If such command is found, the proper address_inserter function is executed and TRUE is returned,
 * Otherwise, FALSE is returned.
 */
bool secondpass_parse_command(char **line_p) {
    operation operation;

    if ((operation = isoperation(line_p)).opcode == -1) {
        return FALSE;
    }
    if (operation.address_inserter(*line_p) == -1) {
        return FALSE;
    }
    return TRUE;
}

/*
 * Tries to find operation in specified string. If an operation is found, it's info is returned, and the specified pointer moved after the operation.
 * Otherwise, unknown operation is returned.
 */
operation isoperation(char **text_p) {
    char *text = *text_p;
    int i = 0, length;
    operation ops[] = {
            {0,  -1, 0, "unknown", NULL},
            {1,  0,  0, "mov",  parse_two_args_command, secondpass_two_args_command},
            {2,  1,  0, "cmp",  parse_two_args_command, secondpass_two_args_command},
            {3,  2,  1, "add",  parse_two_args_command, secondpass_two_args_command},
            {4,  2,  2, "sub",  parse_two_args_command, secondpass_two_args_command},
            {5,  4,  0, "lea",  parse_two_args_command, secondpass_two_args_command},
            {6,  5,  1, "clr",  parse_one_arg_command,  secondpass_one_arg_command},
            {7,  5,  2, "not",  parse_one_arg_command,  secondpass_one_arg_command},
            {8,  5,  3, "inc",  parse_one_arg_command,  secondpass_one_arg_command},
            {9,  5,  4, "dec",  parse_one_arg_command,  secondpass_one_arg_command},
            {10, 9,  1, "jmp",  parse_one_arg_command,  secondpass_one_arg_command},
            {11, 9,  2, "bne",  parse_one_arg_command,  secondpass_one_arg_command},
            {12, 9,  3, "jsr",  parse_one_arg_command,  secondpass_one_arg_command},
            {13, 12, 0, "red",  parse_one_arg_command,  secondpass_one_arg_command},
            {14, 13, 0, "prn",  parse_one_arg_command,  secondpass_one_arg_command},
            {15, 14, 0, "rts",  fill_zero_args_command, secondpass_zero_arg_command},
            {16, 15, 0, "stop", fill_zero_args_command, secondpass_zero_arg_command}
    };

    for (; i < (sizeof(ops) / sizeof(operation)); i++) {
        if ((!strncmp(text, ops[i].opname, (length = strlen(ops[i].opname))))) {
            if (!isspace(text[length])) {
                log_message("Missing white character after operation name. Operation name: %s", ops[i].opname);
                return ops[0];
            }
            skip_characters(text_p, length);
            return ops[i];
        }
    }

    log_message("Entered unknown operation");
    return ops[0];
}

/*
 * Tries to read 2 arguments separated by ',' from specified string, after trimming white characters.
 * If an argument is found, it is copied to a new string, allocated by dynamic memory.
 * A struct containing the arguments found, is returned.
 */
arguments read_args(char *line) {
    arguments args;
    char *token, *arg1 = NULL, *arg2 = NULL;

    if ((token = strtok(line, ", \t\n\0"))) {
        arg1 = token;
        if ((token = strtok(NULL, ", \t\n\0"))) {
            arg2 = token;
        }
        else {
            log_message("Managed to read only 1 operands from 2");
        }
    }
    else {
        log_message("Managed to read only 0 operands from 2");
    }

    if (strtok(NULL, ", \t\n\0")) {
        log_message("Found 3 arguments in command. There are at most 2 argument per command");
        args.arg1 = NULL;
        args.arg2 = NULL;
        return args;
    }

    args.arg1 = arg1;
    args.arg2 = arg2;

    return args;
}

/*
 * Reads argument from specified string, after trimming white characters.
 * If argument is found, it is copied to a new string, allocated by dynamic memory, and a pointer to it's beginning is returned.
 * Otherwise NULL is returned.
 */
char *read_arg(char *line) {
    char *token;
    char *arg = NULL;
    token = strtok(line, " \t\n\0");

    if (token) {
        arg = token;

        if (strtok(NULL, " \t\n\0")) {
            log_message("Found 2 arguments in a 1 argument command");
            return NULL;
        }
        return arg;
    }

    log_message("Managed to read only 0 operands from a 1 argument command");
    return arg;
}

/*
 * Initializes A R E flags in the specified command
 */
void fill_flags(command_template *command, int a, int r, int e) {
    command->A = a;
    command->R = r;
    command->E = e;
}

/*
 * Checks if string is a register. If it is, it returns the register's number, otherwise it returns -1
 */
int isregister(char *arg) {
    int reg;

    if (*arg == REG_SUFFIX) {
        if (try_parse_number(arg, &reg, FALSE)) {
            if ((reg >= MIN_REG) && (reg <= MAX_REG)) {
                return reg;
            }
            else {
                log_message("Entered a non existing register. Register: r%d", *arg);
                return -2;
            }
        }
        else {
            /* register suffix is not followed by a valid number. this could be a label, no logs are printed */
            return -2;
        }
    }

    return -1;
}

/*
 * Checks if specified string is a valid integer. If it is a number, the specified integer pointer is initialized to the number found,
 * and 1 is returned
 * Otherwise 0 is returned.
 */
int try_parse_number(char *arg, int *number, int check_suffix) {
    int i = 1;
    int num = 0;
    int sign = 1;

    if (check_suffix && (*arg != NUMBER_SUFFIX)) {
        return FALSE;
    }

    if (arg[i] == NEGATIVE_NUMBER_SUFFIX) {
        sign = -1;
        i++;
    }
    else if (arg[i] == POSITIVE_NUMBER_SUFFIX) {
        i++;
    }

    for (; arg[i] != '\0'; ++i) {
        if (isdigit(arg[i])) {
            num *= 10;
            num += (arg[i] - '0');
        }
        else {
            return FALSE;
        }
    }

    *number = num * sign;
    return TRUE;
}

/*
 * Returns 1 if specified string is a valid address which is '&' followed by a valid label.
 * Returns 0 otherwise.
 */
int isaddress(char *arg) {
    if (*arg == ADDRESS_SUFFIX) {
        arg++; /* skip '&' */
        if (islable(arg, (int) strlen(arg))) {
            return TRUE;
        }
        printf("Not a valid label after \'&\' sign. sign: %s\n", arg);
        return FALSE;
    }
    return FALSE;
}


