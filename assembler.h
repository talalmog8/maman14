#include<stdio.h>
#include <stdarg.h>
#include "labelslist.h"
#include "command_template.h"
#include "firstpass_parsers.h"

/*
 * Macros for basic characters that need to be recognized
 */
#define COMMENT ';'
#define NEWLINE '\n'
#define TAB '\t'
#define STRING_END '\0'
#define SPACE ' '
#define COLON ':'

/*
 * Max source file line length
 */
#define MAX_LINE_LENGTH 81

/*
 * Max source file's name length
 */
#define MAX_FILE_NAME_LENGTH 100

/*
 * Max commands possible in program's output
 * Max guides possible in program's output
 */
#define MAX_PROGRAM_OUTPUT_SIZE 200

typedef enum {
    FALSE,
    TRUE
} bool;

/*
 * Different guide operations types
 */
typedef enum {
    __string,
    __data,
    __extern,
    __entry
} guide_names;


/* command line arguments */
char *nextArg(int amount, char *args[]);


/* File IO Functions */
FILE *openfile_for_read(char *name);
FILE *lazyopen(FILE *file, char *filename, char *postfix);
FILE *openfile_for_write(char *name, char *postfix);
char *readline(FILE *file, char *line);
void disposefile(FILE *file);


/* Parsing */
bool firstpass(FILE *file);
bool secondpass(FILE *file);
bool firstpass_parse_command(char **line_p);
bool secondpass_parse_command(char **line_p);

/* Labels Related */
int findlable(char *line, bool atStart);
bool parselable(char *line, int length, char *output);
char *allocate_label(int length);
bool islable(char *line, int length);
/* todo make one more header file for this functions */


/* addressing types validation */
bool is_origin_address_type_valid(int operation_id, int address_type);
bool is_destination_address_type_valid(int operation_id, int address_type);

/*second pass */
int secondpass_two_args_command(char *text);
int secondpass_one_arg_command(char *text);
int secondpass_zero_arg_command(char *text);

/* guides */
int is_guide(char **line_p);
bool parse_guide(char *line, guide_names guide_type);

/* utilities functions*/
bool is_comment_or_empty(char *line);
bool is_space(char x);
bool is_end(char x);
void skip_white_characters(char **text_p);
void skip_characters(char **text_p, int amount);

/* registers operations */
void setIC(unsigned int ic);
unsigned int getIC(void);
void incIC(unsigned int addition);
void setDC(unsigned int dc);
unsigned int getDC(void);
void incDC(unsigned int addition);


/* binary output arrays */
void assign_output_arrays(void);
void reset_output_arrays(void);
void dispose_output_arrays(void);
void print_output_arrays(char *filename, unsigned int ic, unsigned int dc);
guide_template *get_current_guide(void);
command_template *get_current_command(void);
command_template *get_command_by_ic(unsigned int ic);

/* externals list */
typedef struct external_node {
    unsigned int location;
    char *label;
    struct external_node *next;
} external_node;
external_node *iterate_externals(void);
void add_external(char *label, int location);
void dispose_externals(void);

/* output files for entries and externals */
void printentries(char *filename, labelnode *head);
void printexternals(char *filename, external_node *head);

/*
 * Logger functions
 */
void dispose_logger(void);
void set_logger(char *filename, char *log_stage);
void log_message(char *fmt, ...);
void set_logger_current_line(char *current_line, unsigned int line_no);
void dispose_logger_current_line(void);