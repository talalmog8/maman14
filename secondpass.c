#include <stdlib.h>
#include "assembler.h"

/*
 * Performs the secondpass of assembler on current source file
 */
bool secondpass(FILE *file) {
    bool output = TRUE; /* will be FALSE if there is an error in firstpass */
    char *moving_line;  /* holds current position in the line that is being parsed */
    char *label; /* points to the label found */
    int label_length; /* holds current position in the line that is being parsed */
    int guide_result; /* holds the guide's type found*/
    char line_mem[MAX_LINE_LENGTH]; /* reused for holding current line read from source file */
    int line_count = 0;

    /* initialize registers */
    setIC(100);
    setDC(0);
    while (readline(file, (moving_line = line_mem)) != NULL) {
        skip_white_characters(&moving_line);
        set_logger_current_line(moving_line, ++line_count);

        if (is_comment_or_empty(moving_line)) {
            continue; /* Skip empty lines and comments */
        }
        else if ((label_length = findlable(moving_line, TRUE)) != -1) {
            /* skip label at beginning of line */
            skip_characters(&moving_line, label_length + 1);
        }

        guide_result = is_guide(&moving_line);
        if ((guide_result == __string) || (guide_result == __data) || (guide_result == __extern)) {
            continue; /* already parsed commands */
        }
        else if (guide_result == __entry) {
            /* this block will updates current label's metadata. need to strore label as label_entry    */
            if ((label_length = findlable(moving_line, FALSE)) == -1) {
                log_message("Missing label \\ Illegal label on entry guide");
                output = FALSE;
            }
            else if (parselable(moving_line, label_length, (label = allocate_label(label_length)))) {
                if (!update_label_kind(label, label_entry)) {
                    log_message("Couldn't find label symbols table, in order to update it's kind to entry. label: %s",
                                label);
                    output = FALSE;
                }
                free(label);
                skip_characters(&moving_line, label_length);
                if (!is_end(*moving_line)) {
                    log_message("Excess data in command");
                }
            }

        }
        else if (guide_result == -1) {
            /* this should be command operation*/
            output &= secondpass_parse_command(&moving_line);
        }
        else {
            /* entered an unknown guide starting with '.' */
            output = FALSE;
        }
    }

    dispose_logger_current_line();
    return output;
}