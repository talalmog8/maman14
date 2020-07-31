#include<stdio.h>
#include "lableslist.h"
#include "command_template.h"
#include "operations_parsers.h"

#define MAX_LINE_LENGTH 81
#define MAX_FILE_NAME_LENGTH 100
#define MAX_PROGRAM_OUTPUT_SIZE 200

typedef enum {
    FALSE,
    TRUE    
} bool;

typedef enum {
    CORRECT,
    OTHER,
    ERROR
} parsing_result;


typedef enum {
    __string,
    __data,
    __extern,
    __entry
} guide_names;


/* command line arguements */
char * nextArg(int amount, char* args[]);
FILE * readFile(char *name);


/* file */
FILE * openfile(char *name);
char *readline(FILE *file, char* line);
void disposefile(FILE *file);

/* parsing */
bool firstpass(FILE *file);


/* labels */
int findlable(char * line, bool atStart);
bool parselable(char *line, int length, char *output);
char * allocate_label(int length);
bool islable(char *line, int length);

/* command */
bool parse_command(char **line_p);

/* guide */
int is_guide(char **line_p);
bool parse_guide(char *line, guide_names guide_type);

/* utilities functions*/
bool is_comment_or_empty(char *line);
bool is_space(char  x);
bool is_end(char  x);
void skip_white_characters(char **text_p);
void skip_characters(char **text_p, int amount);

/* registers operations */
void setIC(int ic);
unsigned int getIC();
void incIC(unsigned  int addition);
void setDC(int dc);
unsigned int getDC();
void incDC(unsigned  int addition);


/* binary output arrays */
void assign_output_arrays(void);
void append_command(command_template command);
void reset_output_arrays(void);
void dispose_output_arrays(void);
void print_output_arrays(void);
guide_template* get_current_guide();
command_template* get_current_command();
command_template* peek_last_command();