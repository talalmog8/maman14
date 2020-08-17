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
    arguments args = read_args(text);
    incIC(1);

    if (args.arg1 && args.arg2) {
        if (parse_arg(args.arg1) != -1) {
            if (parse_arg(args.arg2) != -1) {
                parsed = 1;
            } else {
                fprintf(stderr, "Failed to parse second argument in command. arguments: %s\n", text);
            }
        } else {
            fprintf(stderr, "Failed to parse first argument in command. arguments: %s\n", text);
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
    int addressing_type = -1;

    if (isregister(arg) != -1) {
        addressing_type = REGISTER_ADDRESSING;
    } else if (try_parse_number(arg, &number_arg)) {
        addressing_type = IMMEDIATE_ADDRESSING;
        incIC(1);
    } else if (isaddress(arg)) {
        addressing_type = RELATIVE_ADDRESSING;
        /* todo  take care of operations validtaion. this souldn't happen*/
    } else if (islable(arg, strlen(arg))) {
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
    int addressing_type = -1;
    char *arg;
    incIC(1);

    if (!(arg = read_arg(text))) {
        return -1;
    }

    if (isregister(arg) != -1) {
        addressing_type = REGISTER_ADDRESSING;
    } else if (try_parse_number(arg, &number_arg)) {
        addressing_type = IMMEDIATE_ADDRESSING;
        incIC(1);
    } else if (isaddress(arg)) {
        addressing_type = RELATIVE_ADDRESSING;
        if(!insert_jump(get_command_by_ic(getIC()), arg)){
            addressing_type = -1; /*  this will fail secondpass */
        }
    } else if (islable(arg, strlen(arg))) {
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
static int insert_jump(command_template *command, char *arg){
    labelnode *label;
    int jump;

    if(!(label = get_label(arg + 1))){
        fprintf(stderr, "Label's info not found.\n");
        return FALSE;
    }

    if(label -> location == 0){
        fprintf(stderr, "Cannot jump to external labels.\n");
        return FALSE;
    }

    jump = ((label -> location) - ((int)getIC() - 1));

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
    labelnode *label_info;

    if (!(label_info = get_label(label))) {
        fprintf(stderr, "Could not find label by the name: %s, in label list.\n", label);
        return FALSE;
    }

    if (label_info->kind == label_external) {
        fill_flags(command, FALSE, FALSE, TRUE);
        add_external(label, getIC());
    } else {
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
