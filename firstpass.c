#include "assembler.h"

/*
    Performs the firstpass of assembler on current source file
*/
bool firstpass(FILE *file) {
    bool output = TRUE; /* will be FALSE if there is an error in firstpass */
    bool foundLabel; /* TRUE if label is found*/
    char *moving_line; /* holds current position in the line that is being parsed */
    char *label; /* points to label found */
    int label_length; /* holds label's length */
    int guide_result = 0;  /*  */
    char line_mem[MAX_LINE_LENGTH]; /* reused for holding current line read from source file */

    /* initialize registers */
    setIC(100);
    setDC(0);
    while (readline(file, (moving_line = line_mem)) != NULL) {
        foundLabel = FALSE;
        skip_white_characters(&moving_line);
        set_logger_current_line(line_mem);

        if (is_comment_or_empty(moving_line)) {
            continue; /* Skip empty lines and comments */
        } else if ((label_length = findlable(moving_line, TRUE)) !=
                   -1) { /* This block is for adding a label at beginning of line */
            /* found label's delimiter at beginning of command */
            if (parselable(moving_line, label_length, (label = allocate_label(label_length)))) {
                /* label has been validated successfully */
                foundLabel = TRUE;
                skip_characters(&moving_line, label_length + 1); /* forwards line after the label for further parsing */
            } else {
                log_message("Found illegal label in line: %s", line_mem);
                output = FALSE;
            }
        }

        /*
         * This block starts parsing of guide, if present
         */
        guide_result = is_guide(&moving_line);
        if ((guide_result == __string) || (guide_result == __data)) {
            /* found a .string or .data operation */
            if (foundLabel == TRUE)
                output &= add_label_if_new(label, getDC(), label_data, label_no_kind); /* add label to symbol table */
            output &= parse_guide(moving_line, guide_result); /* parse the rest of guide command */
        } else if (guide_result == __entry) {
            continue; /* parsed at secondpass */
        } else if (guide_result == __extern) {
            if ((label_length = findlable(moving_line, FALSE)) == -1) {
                log_message("Missing label / Illegal label format, on extern guide command");
                output = FALSE;
            } else if (parselable(moving_line, label_length, (label = allocate_label(label_length)))) {
                /* found a valid label in .extern guide */
                addtoend(label, 0, label_data, label_external); /* add label to symbol table */
                skip_characters(&moving_line, label_length);
                if (!is_end(*moving_line)) {
                    log_message("Excess data in the end of extern guide");
                }
            }
        } else if(guide_result == -1) {
            /* this should be a command. starting command parsing on line*/
            if (foundLabel)
                output &= add_label_if_new(label, getIC(), label_code, label_no_kind); /* add label to symbol table */

            output &= firstpass_parse_command(&moving_line); /* parse the rest of the command */
        }
        else{
            /* entered an unknown guide starting with '.' */
            output = FALSE;
        }
    }

    increment_data_labels(getIC()); /* updates symbols table at end of firstpass */
    dispose_logger_current_line();
    return output;
}