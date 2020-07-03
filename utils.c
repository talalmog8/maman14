int stringlen(char *string);

int stringlen(char *string){
    int counter = 1;
    while (*string != '\0')            
        ++counter;
    return counter;
}
