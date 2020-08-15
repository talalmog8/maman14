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
    
    /* intializes registers */
    setIC(100);
    setDC(0);    
    while (readline(file, (moving_line = line_mem)) != NULL) {
        foundLabel = FALSE;
        skip_white_characters(&moving_line);

        if (is_comment_or_empty(moving_line)) {
            continue;
        } 
        else if ((label_length = findlable(moving_line, TRUE)) != -1) {   
            /* found label's delimeter at beginning od command */         
            if (parselable(moving_line, label_length, (label = allocate_label(label_length)))) {
                /* label has been validated successfuly */
                foundLabel = TRUE;
                skip_characters(&moving_line, label_length + 1); /* foward line after the label*/
            } 
            else {
                fprintf(stderr, "Found illegal label in line: %s", line_mem);
                output = FALSE;
            }
        }

        if (((guide_result = is_guide(&moving_line)) == __string) || guide_result == __data) {
            /* found a .string or .data operation */
            if (foundLabel == TRUE) 
                output &= add_label_if_new(label, getDC(), label_data, label_no_kind); /* add label to symbol table */
            output &= parse_guide(moving_line, guide_result); /* parse the rest of guide command */
        }
        else if (guide_result == __entry) {
            continue; /* parsed at secondpass */
        }
        else if (guide_result == __extern) {            
            if ((label_length = findlable(moving_line, FALSE)) == -1) {
                printf("Missing label on extern guide command\n");
                output = FALSE;
            }
            else if (parselable(moving_line, label_length, (label = allocate_label(label_length)))) {
                /* found a valid label in .extern guide */
                addtoend(label, 0, label_data, label_external); /* add label to symbol table */
                skip_characters(&moving_line, label_length);
                if (!is_end(*moving_line)) {
                    fprintf(stderr,"Too much data in command: %s", line_mem);
                }
            }
        } 
        else {
            /* this should be command line*/
            if(foundLabel)
                output &= add_label_if_new(label, getIC(), label_code, label_no_kind); /* add label to symbol table */

            output &= firstpass_parse_command(&moving_line); /* parse the rest of the command */
        }
    }

    increment_data_labels(getIC()); /* updates symbols table at end of firstpass */
    return output;
}