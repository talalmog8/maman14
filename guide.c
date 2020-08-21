#include <string.h>
#include "assembler.h"
#include<ctype.h>

static bool parse_numbers(char *line);

static bool parse_number(char *text);

static bool parse_string(char *text);

/*
    Check if specified string contains a guide type command. 
    If a valid guide type is found, the proper guide_name is returned.
    Otherwise, -1 is returned 
*/
int is_guide(char **line_p) {
    int i;
    char *line = (*line_p);

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
            if(is_space(line[types[i].length])){
                skip_characters(line_p, types[i].length);
                return types[i].no;
            }
            else{
                log_message("Missing whit character after guide name. guide name: %s", types[i].type);
                return -1;
            }
        }
    }

    if(**line_p == '.'){
        log_message("Unknown guide");
        return -2;
    }

    return -1;
}

/*
    Parses specified string as guide command 
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

    token = strtok(line, ", \t\n\0");

    while (token != NULL) {
        if (parse_number(token)) {
            counter++;
        }

        token = strtok(NULL, ", \t\n\0");
    }

    incDC(counter);
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

    if (*text == '-') {
        sign = -1;
        i++;
    } else if (*text == '+') {
        i++;
    }

    for (; text[i] != '\0'; ++i) {
        if (isdigit(text[i])) {
            num *= 10;
            num += (text[i] - '0');
        } else {
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
    } else if(*(text+ 1) == '\"'){
        log_message("Empty string int .string command");
    }

    for (i = 1; text[i] != '\"' && text[i] != '\n' && text[i] != '\0'; ++i) {
        if ((text[i] >= 32 && text[i] < 127)) {
            get_current_guide()->data = text[i];;
            incDC(1);
        } else {
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



