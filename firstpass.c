#include "assembler.h"


bool firstpass(FILE *file)
{
    bool output = TRUE;
    bool  foundLabel;
    char *line, *origline, *label;
    int label_length;
    int guide_result = 0;

    setIC(100);
    setDC(0);
    char line_mem[MAX_LINE_LENGTH];
    while (readline(file, (line = origline = line_mem)) != NULL)
    {
        foundLabel = FALSE;
        line += skip_white_characters(line);

        if(is_comment_or_empty(line)){
            continue;
        }
        else if((label_length = findlable(line, TRUE)) != -1){
            label = allocate_label(label_length);
            if(parselable(line, label_length, label)){
                foundLabel = TRUE;
                line+= (label_length + 1); /* foward line after label */
                line += skip_white_characters(line);
            }
            else{
                fprintf(stderr, "Found illegal label in line: %s", line_mem);
                output = FALSE;
            }
        }

        if(((guide_result = is_guide(line)) == __string) || guide_result == __data){
            if(foundLabel == TRUE){
                if(exists(label)){
                    printf("Label already exists\n");
                    output = FALSE;
                }
                else{
                    addtoend(label, getDC(), data, 0);
                }
            }
            output &= parse_guide(line, guide_result);
            if(!output){
                printf("Guide line arguments could not be parsed properly. arguments: %s", line_mem);
            }
        }
        else if (guide_result == __entry){
            printf("Found entry guide. this will be handled in second pass\n");
            continue;
        }
        else if(guide_result == __extern){
            label_length = findlable(line, FALSE);
            if(label_length == -1){
                printf("Missing label on extern guide command\n");
                output = FALSE;
            }
            else if(parselable(line, label_length, (label = allocate_label(label_length)))){
                addtoend(label, 0, data, label_external);
                line+= (label_length); /* foward line after label */
                line += skip_white_characters(line);
                if(*line != '\n' && *line != '\0'){
                    printf("Too much data in command: %s", line_mem);
                }
            }
        }
    }

    printlist();
    print_output_arrays();
    return output;
}
