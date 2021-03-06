#include "assembler.h"

static int parse_arg(char *arg);
static int insert_jump(command_template *command, char *arg);
static int insert_label(command_template *command, char *label);

/*
 * Parses a 2 argument command
 * Returns 1 if succeeded and -1 otherwise.
 */
int secondpass_two_args_command(char *text) {
    int parsed = -1;
    arguments args = read_args(text); /* the arguments stripped from white characters, separated by ',' */
    incIC(1);

    if (args.arg1 && args.arg2) {
        if (parse_arg(args.arg1) != -1) {
            if (parse_arg(args.arg2) != -1) {
                parsed = 1;
            }
            else {
                log_message("Failed to parse second argument in command");
            }
        }
        else {
            log_message("Failed to parse first argument in command");
        }

    }

    return parsed;
}

/*
 * Parses one argument from a 2 argument command in secondpass.
 * Inserts label location needed. Otherwise, increments IC according to command.
 * Returns addressing type found, or -1 if couldn't recognize addressing type.
 */
static int parse_arg(char *arg) {
    int number_arg;
    int addressing_type = -1; /* the addressing type found */

    if (isregister(arg) >= 0) {
        addressing_type = REGISTER_ADDRESSING;
    }
    else if (try_parse_number(arg, &number_arg, TRUE)) {
        addressing_type = IMMEDIATE_ADDRESSING;
        incIC(1);
    }
    else if (isaddress(arg)) {
        addressing_type = RELATIVE_ADDRESSING;
    }
    else if (islable(arg, strlen(arg))) {
        addressing_type = DIRECT_ADDRESSING;
        if (!insert_label(get_command_by_ic(getIC()), arg)) {
            addressing_type = -1; /*  this will fail secondpass */
        }
    }

    return addressing_type;
}

/*
 * Parses specified command for secondpass.
 * Inserts label location or jump to label if needed. Otherwise, increments IC according to command.
 */
int secondpass_one_arg_command(char *text) {
    int number_arg;
    int addressing_type = -1; /* the addressing type found */
    char *arg; /* the argument stripped from with characters */
    incIC(1);

    if (!(arg = read_arg(text))) {
        return -1;
    }

    if (isregister(arg) >= 0) {
        addressing_type = REGISTER_ADDRESSING;
    }
    else if (try_parse_number(arg, &number_arg, TRUE)) {
        addressing_type = IMMEDIATE_ADDRESSING;
        incIC(1);
    }
    else if (isaddress(arg)) {
        addressing_type = RELATIVE_ADDRESSING;
        if (!insert_jump(get_command_by_ic(getIC()), arg)) {
            addressing_type = -1; /*  this will fail secondpass */
        }
    }
    else if (islable(arg, strlen(arg))) {
        addressing_type = DIRECT_ADDRESSING;
        if (!insert_label(get_command_by_ic(getIC()), arg)) {
            addressing_type = -1; /*  this will fail secondpass */
        }
    }
    return addressing_type;
}

/*
 * No parsing is needed for zero arg commands in secondpass.
 * Keeping track of IC is essential for secondpass algorithm
 */
int secondpass_zero_arg_command(char *text) {
    incIC(1);

    return TRUE;
}

/*
 * Tries to insert jump to specified label into current command's output.
 * If label is not found in symbols table, FALSE is returned.
 * Otherwise TRUE is returned.
 */
static int insert_jump(command_template *command, char *arg) {
    labelnode *label; /* stores the label with specified name */
    int jump; /* stores the jump needed to be inserted to command */

    if (!(label = get_label(arg + 1))) {
        log_message("Can't insert jump because label's info not found. label: %s", arg + 1);
        return FALSE;
    }

    if (label->location == 0) {
        log_message("Cannot jump to external labels. Label: %s", arg + 1);
        return FALSE;
    }

    jump = ((label->location) - ((int) getIC() - 1));

    command->opcode = EXTRACT_OPCODE(jump);
    command->func = EXTRACT_FUNC(jump);
    command->des_register = EXTRACT_DES_REG(jump);
    command->des_delivery_type = EXTRACT_DES_DEV_TYPE(jump);
    command->orig_register = EXTRACT_ORIG_REG(jump);
    command->orig_delivery_type = EXTRACT_ORIG_DEV_TYPE(jump);

    fill_flags(command, TRUE, FALSE, FALSE);
    incIC(1);

    return TRUE;
}

/*
 * Tries to insert specified label's location from symbols table to current command's output.
 * Also sets the E flag is the label is external. otherwise, sets the R flag.
 * If label is not in symbols table, FALSE is returned.
 * Otherwise, TRUE is returned.
 */
static int insert_label(command_template *command, char *label) {
    labelnode *label_info; /* stores the label with specified name */

    if (!(label_info = get_label(label))) {
        log_message("Couldn't insert label's location because label is not in symbols table. Label: %s", label);
        return FALSE;
    }

    if (label_info->kind == label_external) {
        fill_flags(command, FALSE, FALSE, TRUE);
        add_external(label, getIC());
    }
    else {
        fill_flags(command, FALSE, TRUE, FALSE);
    }

    command->opcode = EXTRACT_OPCODE(label_info->location);
    command->func = EXTRACT_FUNC(label_info->location);
    command->des_register = EXTRACT_DES_REG(label_info->location);
    command->des_delivery_type = EXTRACT_DES_DEV_TYPE(label_info->location);
    command->orig_register = EXTRACT_ORIG_REG(label_info->location);
    command->orig_delivery_type = EXTRACT_ORIG_DEV_TYPE(label_info->location);
    incIC(1);

    return TRUE;
}
