#include "assembler.h"


bool firstpass(FILE *file)
{
    bool output = TRUE;
    bool  foundLabel = FALSE;
    char *line, *origline, *label;
    int label_length;
    int guide_result = 0;

    setIC(100);
    setDC(0);
    disposelist();
    while ((origline = line = readline(file)) != NULL)
    {
        line += skip_white_characters(line);

        if(is_comment_or_empty(line)){
            continue;
        }
        else if((label_length = findlable(line, TRUE)) != -1){
            label = allocate_label(label_length);
            if(parselable(line, label_length, label)){
                foundLabel = TRUE;
                printf("LABEL in line: %s", origline);
                line+= (label_length + 1); /* foward line after label */
                line += skip_white_characters(line);
            }
            else{
                fprintf(stderr, "Found illegal label in line: %s", line);
                output = FALSE;
            }
        }

        if(((guide_result = is_guide(line)) == __string) || guide_result == __data){
            printf("Found string or data Guide: %s", line);

            if(foundLabel == TRUE){
                if(exists(label)){
                    printf("Label already exists");
                    output = FALSE;
                }
                else{
                    addtoend(label, getDC(), data, 0);
                }
            }
        }
        else if (guide_result == __entry){
            printf("Found entry guide. this will be handled in second pass\n");
            continue;
        }
        else if(guide_result == __extern){
            label_length = findlable(line, FALSE);
            if(label_length == -1){
                printf("Missing label on exten guide command");
                output = FALSE;
            }
            else if(parselable(line, label_length, (label = allocate_label(label_length)))){
                addtoend(label, 0, data, label_external);
                line+= (label_length); /* foward line after label */
                line += skip_white_characters(line);
                if(*line != '\n' && *line != '\0'){
                    printf("Too much data in command: %s", origline);
                }
            }
        }

        disposeline(origline);
    }
    printlist();
    return output;
}
