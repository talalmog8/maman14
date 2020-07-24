#include "assembler.h"

bool firstpass(FILE *file)
{
    bool output = TRUE;
    bool  foundLabel = FALSE;
    char *line, *origline;
    char *end_of_lable;
    int guide_result = 0;
    int i;

    setIC(100);
    setDC(0);
    while ((origline = line = readline(file)) != NULL)
    {
        line += skip_white_characters(line);
        if(is_comment_or_empty(line)){
            continue;
        }
        else if((end_of_lable = findlable(line))){
            if(parselable(line, (end_of_lable - line - 1))){
                foundLabel = TRUE;
                printf("LABEL in line: %s", line);
                line = end_of_lable;
            }
            else{
                fprintf(stderr, "Found illegal label in line: %s", line);
                output = FALSE;
            }

            if((guide_result = is_guide(line)) != -1){
                printf("Found Guide: Type: %d : %s", guide_result, line);
            }
        }

        disposeline(origline);
    }

    return output;
}
