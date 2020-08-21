/*
 * Macros for addressing types
 */
#define EMPTY_FIELD 0
#define IMMEDIATE_ADDRESSING 0
#define DIRECT_ADDRESSING 1
#define RELATIVE_ADDRESSING 2
#define REGISTER_ADDRESSING 3

/*
 * Bitwise operation for extracting elements of command_template
 */
#define EXTRACT_OPCODE(num) ((num) >> 15)
#define EXTRACT_FUNC(num) ((num) & 31)
#define EXTRACT_DES_REG(num) (((num) >> 5) & 7)
#define EXTRACT_DES_DEV_TYPE(num) (((num) >> 8) & 3)
#define EXTRACT_ORIG_REG(num) (((num) >> 10) & 7)
#define EXTRACT_ORIG_DEV_TYPE(num) (((num) >> 13) & 3)

/*
 * Data type that holds information about a command.
 * Also holds pointers to the right a firstpass and secondpass parsers for the command.
 */
typedef struct {
    int id;
    int opcode;
    int funct;
    char *opname;

    int (*parser)(char *text, int opcode, int funct, int operation_id);

    int (*address_inserter)(char *text);
} operation;

/*
 * Data type for holding to string arguments. Used for parsing commands with 2 arguments
 */
typedef struct {
    char *arg1;
    char *arg2;
} arguments;

/*
 * Further documentation is placed where the functions are written
 */

/* Recognizes operations*/
operation isoperation(char **text_p);

/* Argument readers*/
arguments read_args(char *line);

char *read_arg(char *line);


/* Element parsers*/
int try_parse_number(char *arg, int *number, int check_suffix);

int isregister(char *arg);

int isaddress(char *arg);

/* Firstpass command parsers */
int parse_two_args_command(char *text, int opcode, int funct, int operation_id);

int parse_one_arg_command(char *text, int opcode, int funct, int operation_id);

int fill_zero_args_command(char *text, int opcode, int funct, int operation_id);

void fill_flags(command_template *command, int a, int r, int e);