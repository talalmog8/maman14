#include <string.h>
#include "assembler.h"

/*
    Entry point of program. 
    Each loop iteration complies another source file provided in command line arguments.    
*/
int main(int argc, char *argv[]) {
    int firstpass_result; /* stores firstpass result */
    unsigned int icf, idf; /* store registers final count after firstpass */
    char *filename; /* pointer to current file's name */
    FILE *file; /* pointer to current file */
    assign_output_arrays(); /* assigns memory for guides and commands output arrays */
    while ((filename = nextArg(argc, argv)) != NULL) {
        if ((file = openfile_for_read(filename))) {
            /* Opened a source file */
            reset_output_arrays(); /* resets output arrays (that stores compiled data) indexes */
            set_logger(filename, "firstpass"); /* adds "firstpass" suffix to log messages from firstpass */
            if (!(firstpass_result = firstpass(file))) {
                log_message("Failed firstpass");
            }
            rewind(file); /* reset file cursor to beginning for secondpass */
            set_logger(filename, "secondpass"); /* adds "secondpass" suffix to log messages from secondpass */
            /* store final IC and final DC */
            icf = getIC();
            idf = getDC();
            if (secondpass(file) && firstpass_result) {
                /* current source file succeeded in secondpass, output files will be created */
                printexternals(filename, iterate_externals()); /* creates and fills .ext file if needed */
                printentries(filename, iterate_labels()); /* creates and fills .ent file if needed */
                print_output_arrays(filename, icf, idf); /* creates and fills .ob file */
            } else {
                log_message("Failed secondpass");
            }
            dispose_labels(); /* disposes symbols table */
            dispose_externals(); /* disposes externals table */
            disposefile(file); /* disposes current file pointer */
        } else {
            printf("Could not open file: \"%s.as\". It will not be compiled\n", filename);
        }
    }
    dispose_output_arrays(); /* disposes output arrays */
    dispose_logger(); /* disposes logger */
    return 0;
}