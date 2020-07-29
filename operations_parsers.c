#include "assembler.h"

int parse_mov(char *text, int opcode, int funct) {

}

int parse_cmp(char *text, int opcode, int funct) {}

int parse_add(char *text, int opcode, int funct) {}

int parse_sub(char *text, int opcode, int funct) {}

int parse_lea(char *text, int opcode, int funct) {}

int parse_clr(char *text, int opcode, int funct) {}

int parse_not(char *text, int opcode, int funct) {}

int parse_inc(char *text, int opcode, int funct) {}

int parse_dec(char *text, int opcode, int funct) {}

int parse_jmp(char *text, int opcode, int funct) {}

int parse_bne(char *text, int opcode, int funct) {}

int parse_jsr(char *text, int opcode, int funct) {}

int parse_red(char *text, int opcode, int funct) {}

int parse_prn(char *text, int opcode, int funct) {}

int parse_rts(char *text, int opcode, int funct) {}

int parse_stop(char *text, int opcode, int funct) {
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

    return TRUE;
}