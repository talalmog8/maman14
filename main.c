#include <string.h>
#include "assembler.h"

/*
    Entry point of program. 
    Each loop iteration complies another source file provided in command line arguments.    
*/
int main(int argc, char *argv[]) {
    unsigned int icf, idf;
    char *arg;
    FILE *file;
    assign_output_arrays(); /* assigns memory for guides and commands output arrays */
    while ((arg = nextArg(argc, argv)) != NULL) {
        if ((file = openfile_for_read(arg))) {
            /* Opened a source file */
            reset_output_arrays(); /* resest output arrays (that stores compiled data) indexes */
            if (firstpass(file)) { 
                /* current source file succeeded in firstpass */
                rewind(file); /* reset file cursor to beginning for secondapass */
                /* store final IC and final DC */
                icf = getIC();
                idf = getDC();
                if (secondpass(file)) {
                    /* current source file succeeded in secondpass */
                    printexternals(arg, iterate_externals()); /* creates and fills .ext file if needed */
                    printentries(arg, iterate_labels()); /* creates and fills .ent file if needed */
                    print_output_arrays(arg, icf, idf); /* creates and fills .ob file */
                } else {
                    printf("Failed second pass\n");
                }
            } else {
                printf("Failed first pass\n");
            }
            dispose_labels(); /* disposes symbols table */
            dispose_externals(); /* disposes externals table */
            disposefile(file); /* disposes current file pointer */
        } else {
            printf("Could not open file: \"%s.as\". It will not be compiled\n", arg);
        }
    }
    dispose_output_arrays(); /* disposes output arrays */

    return 0;
}