#include "assembler.h"

static int parse_arg(char *arg);

static int insert_jump(command_template *command, char *arg);
static int insert_label(command_template *command, char *label);

int secondpass_two_args_command(char *text) {
    int parsed = 0;
    arguments args = read_args(text);
    incIC(1);

    if (args.arg1 && args.arg2) {
        if (parse_arg(args.arg1) != -1) {
            if (parse_arg(args.arg2) != -1) {
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
        if (!insert_label(get_command_by_ic(getIC()), (arg + 1))) {
            addressing_type = -1;
        }
    } else if (islable(arg, strlen(arg))) {
        addressing_type = DIRECT_ADDRESSING;
        if (!insert_label(get_command_by_ic(getIC()), arg)) {
            addressing_type = -1;
        }
    }

    return addressing_type;
}


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
            addressing_type = -1;
        }
    } else if (islable(arg, strlen(arg))) {
        addressing_type = DIRECT_ADDRESSING;
        if (!insert_label(get_command_by_ic(getIC()), arg)) {
            addressing_type = -1;
        }
    }
    return addressing_type;
}

int secondpass_zero_arg_command(char *text) {
    incIC(1);

    return TRUE;
}

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
