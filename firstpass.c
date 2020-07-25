#include "assembler.h"


bool firstpass(FILE *file)
{
    bool output = TRUE;
    bool  foundLabel = FALSE;
    char *line, *origline, *label;
    int label_length;
    int guide_result = 0;
    int i;

    setIC(100);
    setDC(0);
    disposelist();
    while ((origline = line = readline(file)) != NULL)
    {
        line += skip_white_characters(line);

        if(is_comment_or_empty(line)){
            continue;
        }
        else if((label_length = findlable(line)) != -1){
            label = allocate_label(label_length);
            if(parselable(line, label_length, label)){
                foundLabel = TRUE;
                printf("LABEL in line: %s", origline);
            }
            else{
                fprintf(stderr, "Found illegal label in line: %s", line);
                output = FALSE;
            }
        }


        if(((guide_result = is_guide(line)) == __string) || guide_result == __data){
            printf("Found Guide: Type: %d : %s", guide_result, line);

            if(foundLabel == TRUE){
                if(exists(label)){
                    printf("Label already exists");
                    output = FALSE;
                }
                else{
                    addtoend(label, getDC(), data, entry);
                }
            }
        }

        disposeline(origline);
    }

    return output;
}
