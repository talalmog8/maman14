#define EMPTY_FIELD 0
#define IMMEDIATE_ADDRESSING 0
#define DIRECT_ADDRESSING 1
#define RELATIVE_ADDRESSING 2
#define REGISTER_ADDRESSING 3

#define EXTRACT_OPCODE(num) ((num) >> 15)
#define EXTRACT_FUNC(num) ((num) & 31)
#define EXTRACT_DES_REG(num) (((num) >> 5) & 7)
#define EXTRACT_DES_DEV_TYPE(num) (((num) >> 8) & 3)
#define EXTRACT_ORIG_REG(num) (((num) >> 10) & 7)
#define EXTRACT_ORIG_DEV_TYPE(num) (((num) >> 13) & 3)

typedef struct
{
    int opcode;
    int funct;
    char* opname;
    int (*parser)(char *text, int opcode, int funct);
    int (*label_inserter)(char *text);
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