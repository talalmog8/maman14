#include "assembler.h"

bool secondpass(FILE *file) {
    bool output = TRUE;
    char *moving_line, *label;
    int label_length;
    int guide_result;
    char line_mem[MAX_LINE_LENGTH];

    while (readline(file, (moving_line = line_mem)) != NULL) {
        skip_white_characters(&moving_line);
        if (is_comment_or_empty(moving_line)) {
            continue;
        }
        else if ((label_length = findlable(moving_line, TRUE)) != -1) {
            skip_characters(&moving_line, label_length + 1); /* skip label */
        }

        guide_result = is_guide(&moving_line);
        if ((guide_result == __string) || (guide_result == __data) || (guide_result == __extern)) {
            continue;
        }
        else if (guide_result == __entry) {
            if ((label_length = findlable(moving_line, FALSE)) == -1) {
                printf("Missing label on entry guide command\n");
                output = FALSE;
            }
            else if (parselable(moving_line, label_length, (label = allocate_label(label_length)))) {
                if(!update_label_kind(label, label_entry)){
                    fprintf(stderr, "Couldn't find label in label's linked list, in order to update it's kind to entry. lable: %s", label);
                    output = FALSE;
                }
                skip_characters(&moving_line, label_length);
                if (!is_end(*moving_line)) {
                    fprintf(stderr,"Too much data in command: %s", line_mem);
                }
            }

        }
        else {
            /* this should be command operation*/
            printf(">Ho");
        }
    }

    return output;
}