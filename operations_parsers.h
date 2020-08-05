
typedef struct
{
    int opcode;
    int funct;
    char* opname;
    int (*parser)(char *text, int opcode, int funct);
} operation;

typedef struct {
    char *arg1;
    char *arg2;
} arguments;


operation isoperation(char **text_p);
arguments read_args(char *line);
char *read_arg(char *line);
int try_parse_number(char* arg, int *number);
int isregister(char *arg);
int isaddress(char *arg);


int parse_mov(char *text, int opcode, int funct);
int parse_cmp(char *text, int opcode, int funct);
int parse_add(char *text, int opcode, int funct);
int parse_sub(char *text, int opcode, int funct);
int parse_lea(char *text, int opcode, int funct);
int parse_clr(char *text, int opcode, int funct);
int parse_not(char *text, int opcode, int funct);
int parse_inc(char *text, int opcode, int funct);
int parse_dec(char *text, int opcode, int funct);
int parse_jmp(char *text, int opcode, int funct);
int parse_bne(char *text, int opcode, int funct);
int parse_jsr(char *text, int opcode, int funct);
int parse_red(char *text, int opcode, int funct);
int parse_prn(char *text, int opcode, int funct);
int parse_rts(char *text, int opcode, int funct);
int parse_stop(char *text, int opcode, int funct);