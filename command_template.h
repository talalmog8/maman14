
typedef struct {
    unsigned int A : 1;
    unsigned int R : 1;
    unsigned int E : 1;
    unsigned int func : 5;
    unsigned int des_register : 3;
    unsigned int des_delivery_type : 2;
    unsigned int orig_register : 3;
    unsigned int orig_delivery_type : 2;
    unsigned int opcode :6;
} command_template;