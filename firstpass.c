#include "assembler.h"

static bool add_label_if_new(char *label, int location, int area, int kind);

void movePointer(char **p){
    *p = (*p + 1);
}

bool firstpass(FILE *file) {
    bool output = TRUE;
    bool foundLabel;
    char *moving_line, *label;
    int label_length;
    int guide_result = 0;

    setIC(100);
    setDC(0);
    char line_mem[MAX_LINE_LENGTH];
    while (readline(file, (moving_line = line_mem)) != NULL) {
        foundLabel = FALSE;
        skip_white_characters(&moving_line);

        if (is_comment_or_empty(moving_line)) {
            continue;
        } else if ((label_length = findlable(moving_line, TRUE)) != -1) {
            label = allocate_label(label_length);
            if (parselable(moving_line, label_length, label)) {
                foundLabel = TRUE;
                skip_characters(&moving_line, label_length + 1);
            } else {
                fprintf(stderr, "Found illegal label in line: %s", line_mem);
                output = FALSE;
            }
        }

        if (((guide_result = is_guide(&moving_line)) == __string) || guide_result == __data) {
            if (foundLabel == TRUE)
                output &= add_label_if_new(label, getDC(), label_data, label_no_kind);
            output &= parse_guide(moving_line, guide_result);
        }
        else if (guide_result == __entry) {
            printf("Found entry guide. this will be handled in second pass\n");
            continue;
        }
        else if (guide_result == __extern) {
            label_length = findlable(moving_line, FALSE);
            if (label_length == -1) {
                printf("Missing label on extern guide command\n");
                output = FALSE;
            }
            else if (parselable(moving_line, label_length, (label = allocate_label(label_length)))) {
                addtoend(label, 0, label_data, label_external);
                skip_characters(&moving_line, label_length);
                if (!is_end(*moving_line)) {
                    fprintf(stderr,"Too much data in command: %s", line_mem);
                }
            }
        } else {
            /* this should be command line*/
            if(foundLabel)
                output &= add_label_if_new(label, getIC(), label_code, label_no_kind);

            output &= parse_command(moving_line);
        }
    }

    printlist();
    print_output_arrays();
    return output;
}


static bool add_label_if_new(char *label, int location, int area, int kind) {
    if (exists(label)) {
        fprintf(stderr, "Label already exists. Label: %s\n", label);
        return FALSE;
    } else {
        addtoend(label, location, area, kind);
        return TRUE;
    }
}