#include "assembler.h"

bool secondpass(FILE *file) {
    bool output = TRUE;
    char *moving_line, *label;
    int label_length;
    int guide_result;
    char line_mem[MAX_LINE_LENGTH];

    setIC(100);
    setDC(0);
    while (readline(file, (moving_line = line_mem)) != NULL) {
        skip_white_characters(&moving_line);
        set_logger_current_line(moving_line);

        if (is_comment_or_empty(moving_line)) {
            continue;
        } else if ((label_length = findlable(moving_line, TRUE)) != -1) {
            skip_characters(&moving_line, label_length + 1); /* skip label */
        }

        guide_result = is_guide(&moving_line);
        if ((guide_result == __string) || (guide_result == __data) || (guide_result == __extern)) {
            continue;
        } else if (guide_result == __entry) {
            if ((label_length = findlable(moving_line, FALSE)) == -1) {
                log_message("Missing label \\ Illegal label on entry guide");
                output = FALSE;
            } else if (parselable(moving_line, label_length, (label = allocate_label(label_length)))) {
                if (!update_label_kind(label, label_entry)) {
                    log_message("Couldn't find label in label's linked list, in order to update it's kind to entry. label: %s", label);
                    output = FALSE;
                }
                skip_characters(&moving_line, label_length);
                if (!is_end(*moving_line)) {
                    log_message("Excess data in command");
                }
            }

        } else if(guide_result == -1) {
            /* this should be command operation*/
            output &= secondpass_parse_command(&moving_line);
        }
        else{
            /* entered an unknown guide starting with '.' */
            output = FALSE;
        }
    }

    dispose_logger_current_line();
    return output;
}