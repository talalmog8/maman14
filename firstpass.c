#include "assembler.h"

bool firstpass(FILE *file)
{
    bool output = TRUE;
    bool  foundLabel = FALSE;
    char *line, *origline;
    char *lable_len;
    int counter = 0;
    int i;

    setIC(100);
    setDC(0);
    while ((origline = line = readline(file)) != NULL)
    {
        line += skip_white_characters(line);
        if(is_comment_or_empty(line)){
            continue;
        }
        else if((lable_len = findlable(line))){
            if(parselable(line, (lable_len - line - 1))){
                /* add lable to list if is a valid lable */
                foundLabel = TRUE;
                printf("LABEL in line: %s", line);
            }
            else{
                fprintf(stderr, "Found illegal lable in line: %s", line);
                output = FALSE;
            }
        }

        disposeline(origline);
    }

    return output;
}
