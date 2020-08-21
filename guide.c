#include <string.h>
#include "assembler.h"
#include<ctype.h>

#define WRITEABLE_ASCII_MAX 127
#define WRITEABLE_ASCII_MIN 32

#define POSITIVE_NUMBER '+'
#define NEGATIVE_NUMBER '-'

/*
 * Element parsers
 */
static bool parse_numbers(char *line);
static bool parse_number(char *text);
static bool parse_string(char *text);

/*
    Check if specified string contains a guide type command. 
    If a valid guide type is found, the proper guide_name is returned.
    Otherwise, Negative number is returned according to output
*/
int is_guide(char **line_p) {
    int i;
    char *line = (*line_p); /* points to current line*/

    typedef struct {
        char *type;
        size_t length;
        guide_names no;
    } guide_type;

    guide_type types[] = {
            {".string", 7, __string},
            {".data",   5, __data},
            {".entry",  6, __entry},
            {".extern", 7, __extern}
    };


    for (i = 0; i < sizeof(types) / sizeof(guide_type); ++i) {
        if (!strncmp(line, types[i].type, types[i].length)) {
            if (is_space(line[types[i].length])) {
                /* found valid guide type followed by a white character */
                skip_characters(line_p, types[i].length); /* skip guide type for next parsers */
                return types[i].no;
            }
            else {
                log_message("Missing white character after guide name. guide name: %s", types[i].type);
                return -3;
            }
        }
    }

    if (**line_p == '.') {
        log_message("Unknown guide");
        return -2;
    }

    return -1;
}

/*
    Parses the specified .string / .data guide
*/
bool parse_guide(char *line, guide_names guide_type) {
    if (guide_type == __data) {
        return parse_numbers(line);
    }
    return parse_string(line);
}

/*
 * Need to take care of number validation
 */
static bool parse_numbers(char *line) {
    char *token;
    int counter = 0;
    int i;

    /* this block makes sure the is no consecutive ',' in line */
    for (i = 0; line[i] != '\n' && line[i] != '\0'; ++i) {
        if(line[i] == ',' && line[i + 1] == ','){
            log_message("Consecutive ',' within a .data command");
            return FALSE; /* consecutive ',' */
        }
    }

    token = strtok(line, ", \t\n\0");

    while (token != NULL) {
        if (parse_number(token)) {
            counter++;
        }
        else{
            return FALSE;
        }

        token = strtok(NULL, ", \t\n\0");
    }

    incDC(counter); /* each number increments DC by 1 */

    return counter > 0;
}

/*
    Parses a number from specified string and adds it to guide's output array.
    Returns TRUE if number is valid and otherwise returns FALSE 
*/
static bool parse_number(char *text) {
    int i = 0;
    int num = 0;
    int sign = 1;

    if (*text == NEGATIVE_NUMBER) {
        sign = -1;
        i++;
    }
    else if (*text == POSITIVE_NUMBER) {
        i++;
    }

    for (; text[i] != '\0'; ++i) {
        if (isdigit(text[i])) {
            num *= 10;
            num += (text[i] - '0');
        }
        else {
            log_message("Could not parse a number in .data command. number: %s", text);
            return FALSE;
        }
    }

    num = num * sign;
    get_current_guide()->data = num;
    return TRUE;
}

/*
   Parses the content of a .string command and adds it to guides output array. 
   Returns TRUE if the string is valid, otherwise, returns FALSE
*/
static bool parse_string(char *text) {
    int i;
    skip_white_characters(&text);

    if (*text != '\"') {
        log_message("Missing \" suffix in .string argument");
        return FALSE;
    }
    else if (*(text + 1) == '\"') {
        log_message("Empty string int .string command");
    }

    for (i = 1; text[i] != '\"' && text[i] != '\n' && text[i] != '\0'; ++i) {
        if ((text[i] >= WRITEABLE_ASCII_MIN && text[i] < WRITEABLE_ASCII_MAX)) {
            /* this is a writable ascii character */
            get_current_guide()->data = text[i];;
            incDC(1);
        }
        else {
            log_message("Found illegal character in .string guide. character decimal code: %d", text[i]);
            return FALSE;
        }
    }

    if (text[i] != '\"') {
        log_message("Missing \" postfix in .string argument");
        return FALSE;
    }

    skip_characters(&text, i + 1);
    if (!is_end(*text)) {
        log_message("Excess data after .string guide");
        return FALSE;
    }

    get_current_guide()->data = '\0';
    incDC(1);
    return TRUE;
}



