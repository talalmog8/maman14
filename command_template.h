
typedef struct {
    unsigned int E : 1;
    unsigned int R : 1;
    unsigned int A : 1;
    unsigned int func : 5;
    unsigned int des_register : 3;
    unsigned int des_delivery_type : 2;
    unsigned int orig_register : 3;
    unsigned int orig_delivery_type : 2;
    unsigned int opcode :6;
} command_template;

typedef union {
    command_template command;
    unsigned int command_as_integer;
} command_2_integer;

typedef struct {
    unsigned int data : 24;
} guide_template;

typedef union {
    guide_template guide;
    unsigned int guide_as_integer;
} guide_2_integer;
