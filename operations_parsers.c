#include "assembler.h"


static void fill_two_args_command_defaults(command_template *command, int opcode, int funct);
static void fill_one_arg_command_defaults(command_template *command, int opcode, int funct);
static void fill_empty_command(command_template *command);
static void insert_number_to_command(command_template *command, int number);
static int parse_arg(char *arg, command_template *, bool);

/*
 * Parses a 2 argument command.
 * Returns 1 if successful
 * Otherwise, returns -1
 */
int parse_two_args_command(char *text, int opcode, int funct) {
    int parsed = -1;
    arguments args = read_args(text);
    command_template *command = get_current_command();

    if (args.arg1 && args.arg2) {
        fill_two_args_command_defaults(command, opcode, funct);
        if (parse_arg(args.arg1, command, FALSE) != -1) {
            if (parse_arg(args.arg2, command, TRUE) != -1) {
                parsed = 1;
            } else {
                fprintf(stderr, "Failed to parse second argument in command. arguments: %s", text);
            }
        } else {
            fprintf(stderr, "Failed to parse first argument in command. arguments: %s", text);
        }

    }

    return parsed;
}

/*
 * Parses one argument in a 2 argument command
 * If successful, returns addressing type, otherwise, returns -1
 */
static int parse_arg(char *arg, command_template *command, bool is_destination) {
    int number_arg;

    int _register = EMPTY_FIELD;
    int addressing_type = -1;

    if ((_register = isregister(arg)) != -1) {
        addressing_type = REGISTER_ADDRESSING;
    } else if (try_parse_number(arg, &number_arg)) {
        addressing_type = IMMEDIATE_ADDRESSING;
        insert_number_to_command(get_current_command(), number_arg);
    } else if (isaddress(arg)) {
        addressing_type = RELATIVE_ADDRESSING;
        fill_empty_command(get_current_command()); /* reserved for label's address */
        incIC(1); /* saved space for label address*/
    } else if (islable(arg, strlen(arg))) {
        addressing_type = DIRECT_ADDRESSING;
        fill_empty_command(get_current_command()); /* reserved for label's address */
        incIC(1); /* saved space for label address*/
    }

    if (is_destination) {
        command->des_delivery_type  = addressing_type;
        command->des_register = (_register == -1) ? EMPTY_FIELD : _register;
    } else {
        command->orig_delivery_type = addressing_type;
        command->orig_register = (_register == -1) ? EMPTY_FIELD : _register;
    }

    return addressing_type;
}

/*
 * Helper function to fill default field of a 2 args command
 * Increments IC
 * Sets the A flag
 */
static void fill_two_args_command_defaults(command_template *command, int opcode, int funct){
    command->opcode = opcode;
    command->func = funct;
    fill_flags(command, TRUE, FALSE, FALSE);
    incIC(1);
}

/*
 * Parses a one argument command
 * If successful returns the addressing type found.
 * Otherwise, returns -1
 */
int parse_one_arg_command(char *text, int opcode, int funct) {
    int _register, number_arg;
    int addressing_type = -1;
    command_template *command = get_current_command();
    char *arg;

    if (!(arg = read_arg(text))){
        return -1;
    }

    fill_one_arg_command_defaults(command, opcode, funct);
    if ((_register = isregister(arg)) != -1) {
        command->des_register = _register;
        command->des_delivery_type = addressing_type = REGISTER_ADDRESSING;
    } else if (try_parse_number(arg, &number_arg)) {
        command->des_register = EMPTY_FIELD;
        command->des_delivery_type = addressing_type = IMMEDIATE_ADDRESSING;
        insert_number_to_command(get_current_command(), number_arg);
    } else if (isaddress(arg)) {
        command->des_register = EMPTY_FIELD;
        command->des_delivery_type = addressing_type = RELATIVE_ADDRESSING;
        fill_empty_command(get_current_command()); /* reserved for label's address */
        incIC(1);
    } else if (islable(arg, strlen(arg))) {
        command->des_register = EMPTY_FIELD;
        command->des_delivery_type = addressing_type = DIRECT_ADDRESSING;
        fill_empty_command(get_current_command());
        incIC(1); /* saved space for label address*/
    }


    return addressing_type;
}

/*
 * Helper function to fill default fields of one argument commands
 * Increments IC
 * Sets the A flag
 */
static void fill_one_arg_command_defaults(command_template *command, int opcode, int funct) {
    command->opcode = opcode;
    command->func = funct;
    fill_flags(command, TRUE, FALSE, FALSE);
    command->orig_register = EMPTY_FIELD;
    command->orig_delivery_type = EMPTY_FIELD;
    incIC(1);
}

/*
 * Parses a command with zero argument
 * Increments IC
 * Sets the A flag
 */
int fill_zero_args_command(char *text, int opcode, int funct) {
    command_template *command = get_current_command();
    command->opcode = opcode;
    command->func = funct;
    command->orig_register = EMPTY_FIELD;
    command->orig_delivery_type = EMPTY_FIELD;
    command->des_register = EMPTY_FIELD;
    command->des_delivery_type = EMPTY_FIELD;
    fill_flags(command, TRUE, FALSE, FALSE);
    incIC(1);

    if (!is_end(*text)) {
        fprintf(stderr, "Excess text in command.text: %s", text);
        return FALSE;
    }
    return TRUE;
}

/*
 * Inserts a number to the form of command_template using bitwise operations
 * Sets the A flag on
 * Increments IC
 */
static void insert_number_to_command(command_template *command, int number) {
    command->opcode = EXTRACT_OPCODE(number);
    command->func = EXTRACT_FUNC(number);
    command->des_register = EXTRACT_DES_REG(number);
    command->des_delivery_type = EXTRACT_DES_DEV_TYPE(number);
    command->orig_register = EXTRACT_ORIG_REG(number);
    command->orig_delivery_type = EXTRACT_ORIG_DEV_TYPE(number);
    fill_flags(command, TRUE, FALSE, FALSE);
    incIC(1);
}

/*
 * Fills the specified command with zeros. This method is used for debugging secondpass
 */
static void fill_empty_command(command_template *command) {
    command->opcode = 0;
    command->des_register = 0;
    command->des_delivery_type = 0;
    command->orig_register = 0;
    command->orig_delivery_type = 0;
    fill_flags(command, 0, 0, 0);
    command->func = 0;
}

