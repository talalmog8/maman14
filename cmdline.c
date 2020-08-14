#include "assembler.h"

/*
    Returns Next Command Line Argument. When No Arguments Left, Returns NULL
*/
char * nextArg(int amount, char* args[]){
    static int currentArg = 1;  /* start after program's name*/
    return args[currentArg++];
}

