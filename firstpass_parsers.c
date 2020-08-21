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
int parse_two_args_command(char *text, int opcode, int funct, int operation_id) {
    int parsed = -1, temp_parse_result;
    arguments args = read_args(text);
    command_template *command = get_current_command();

    if (args.arg1 && args.arg2) {
        fill_two_args_command_defaults(command, opcode, funct);
        if (is_origin_address_type_valid(operation_id, temp_parse_result = parse_arg(args.arg1, command, FALSE))) {
            if (is_destination_address_type_valid(operation_id,
                                                  temp_parse_result = parse_arg(args.arg2, command, TRUE))) {
                parsed = 1;
            }
            else if (temp_parse_result == -1) {
                log_message("Failed to parse destination addressing type in command.");
            }
            else {
                log_message("Command destination addressing type is not valid, Addressing type found: %d.",
                            temp_parse_result);
            }
        }
        else if (temp_parse_result == -1) {
            log_message("Failed to parse origin addressing type in command");
        }
        else {
            printf("Command origin addressing type is not valid, Addressing type found: %d.",
                   temp_parse_result);
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

    if ((_register = isregister(arg)) >= 0) {
        addressing_type = REGISTER_ADDRESSING;
    }
    else if (try_parse_number(arg, &number_arg, TRUE)) {
        addressing_type = IMMEDIATE_ADDRESSING;
        insert_number_to_command(get_current_command(), number_arg);
    }
    else if (isaddress(arg)) {
        addressing_type = RELATIVE_ADDRESSING;
        fill_empty_command(get_current_command()); /* reserved for label's address */
        incIC(1); /* saved space for label address*/
    }
    else if (islable(arg, strlen(arg))) {
        addressing_type = DIRECT_ADDRESSING;
        fill_empty_command(get_current_command()); /* reserved for label's address */
        incIC(1); /* saved space for label address*/
    }

    if (is_destination) {
        command->des_delivery_type = addressing_type;
        command->des_register = (_register == -1) ? EMPTY_FIELD : _register;
    }
    else {
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
static void fill_two_args_command_defaults(command_template *command, int opcode, int funct) {
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
int parse_one_arg_command(char *text, int opcode, int funct, int operation_id) {
    int _register, number_arg;
    int addressing_type = -1;
    command_template *command = get_current_command();
    char *arg;

    if (!(arg = read_arg(text))) {
        return -1;
    }

    fill_one_arg_command_defaults(command, opcode, funct);
    if ((_register = isregister(arg)) != -1) {
        command->des_register = _register;
        command->des_delivery_type = addressing_type = REGISTER_ADDRESSING;
    }
    else if (try_parse_number(arg, &number_arg, TRUE)) {
        command->des_register = EMPTY_FIELD;
        command->des_delivery_type = addressing_type = IMMEDIATE_ADDRESSING;
        insert_number_to_command(get_current_command(), number_arg);
    }
    else if (isaddress(arg)) {
        command->des_register = EMPTY_FIELD;
        command->des_delivery_type = addressing_type = RELATIVE_ADDRESSING;
        fill_empty_command(get_current_command()); /* reserved for label's address */
        incIC(1);
    }
    else if (islable(arg, strlen(arg))) {
        command->des_register = EMPTY_FIELD;
        command->des_delivery_type = addressing_type = DIRECT_ADDRESSING;
        fill_empty_command(get_current_command());
        incIC(1); /* saved space for label address*/
    }
    else if (addressing_type == -1) {
        log_message("Failed to parse destination addressing type in command");
        return -1; /* Couldn't find addressing type */
    }

    if (!is_destination_address_type_valid(operation_id, addressing_type)) {
        log_message("Command destination addressing type is not valid, Addressing type found: %d.",
                    addressing_type);
        return -1; /* addressing type not valid */
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
int fill_zero_args_command(char *text, int opcode, int funct, int operation_id) {
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

