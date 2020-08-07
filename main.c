#include <string.h>
#include "assembler.h"

int main(int argc, char *argv[]){     
    char *arg;
    FILE *file;
    assign_output_arrays();
    printf("Reading Command Line Arguements For Program: \"%s\" Number Of Arguements: %d\n", argv[0], argc);
    while((arg = nextArg(argc, argv)) != NULL){        
      printf("Next Command Line Arguement: %s\n", arg);              
      if((file = openfile(arg))){
        printf("File Opened: %s\n", arg);
        reset_output_arrays();
        if(!firstpass(file)){
            printf("Failed first pass\n");
        }
        else{
            printlist();
            print_output_arrays();
            printf("ICF: %d\n", getIC());
            printf("IDC: %d\n", getDC());
        }
        disposelist();
        disposefile(file);
        printf("Disposed File: \"%s.as\"\n", arg);
      }
      else{
        printf("Could not open file: \"%s.as\". It will not be compiled\n", arg);
      }
    }    

    dispose_output_arrays();
    return 0;
}