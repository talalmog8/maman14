#include "assembler.h"
#include <string.h>
#include <ctype.h>

/*
    Gets text that could contain a lable. 
    Don't send null or empty text
    Returns 0 if lable is recognized an validated, else returns error code indicating what went wrong
    codes:
    0 = success
    1 = first character is not a letter
    2 = not containing ':'
    3 = additional text after ':' perhaps missing white character after lable or illegal lable characters 
    4 = illegal lable characters
*/
int isLable(char * text){
    int i = 0;             
    int result = 1; /* first character is not a letter */
    
    if(isalpha(text[i])){                
        while (isalnum(text[i]))
        {
            i++;                     
        }       
        if(text[i] == '\0'){            
            result = 2; /* no a lable no containing : */
        }
        else if(text[i] == ':'){
            if(text[i + 1] == '\0'){
                result = 0; /* legit lable*/
            }
            else{
                result = 3; /* additional text after ':' perhaps missing white character after lable or illegal lable characters */
            }
        }        
        else{
            result = 4; /* illegal lable characters */
        }
    }

    return result;
}
