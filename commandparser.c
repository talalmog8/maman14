/*
parsing_result parse_command(char *line){
    char *lable_result;
    parsing_result result = OTHER;
    line += skip_white_characters(line);

    if(lable_result = findlable(line)){
        if(parselable(line, (lable_result - line - 1))){

            printf("FOUND LABLE\n");
        }
        else{
            return ERROR;
        }
    }

    if(isoperation(line) == unknown){
        fprintf(stderr, "entered unknown operation in line %s", line);
        return ERROR;
    }
    
    return result;
}

*/


