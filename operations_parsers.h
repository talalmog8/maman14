
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
void dispose_operands(arguments args);
char *read_arg(char *line);

int try_parse_number(char* arg, int *number);
int isregister(char *arg);
int isaddress(char *arg);

int parse_two_args_command(char *text, int opcode, int funct);
int parse_one_arg_command(char *text, int opcode, int funct);
int fill_zero_args_command(char *text, int opcode, int funct);