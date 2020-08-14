#include <string.h>
#include "assembler.h"

int main(int argc, char *argv[]) {
    unsigned int icf, idf;
    char *arg;
    FILE *file;
    assign_output_arrays();
    printf("Reading Command Line Arguments For Program: \"%s\" Number Of Arguments: %d\n", argv[0], argc);
    while ((arg = nextArg(argc, argv)) != NULL) {
        if ((file = openfile_for_read(arg))) {
            printf("File Opened: %s\n", arg);
            reset_output_arrays();
            if (firstpass(file)) {
                rewind(file);
                icf = getIC();
                idf = getDC();
                if (secondpass(file)) {
                    printentries(arg, iterate_labels());
                    print_output_arrays(arg, icf, idf);
                } else {
                    printf("Failed second pass\n");
                }
            } else {
                printf("Failed first pass\n");
            }
            disposelist();
            disposefile(file);
        } else {
            printf("Could not open file: \"%s.as\". It will not be compiled\n", arg);
        }
    }
    dispose_output_arrays();
    return 0;
}