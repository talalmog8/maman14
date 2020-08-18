#include "assembler.h"

#define UNKNOWN_COMMAND_ID 0
#define CLR_ID 6

bool is__origin_address_type_valid(int operation_id, int address_type){
    int i, *valids;


    int valid_addressing_types[5][4] = {
            {0, 1, 3, -1},
            {0, 1, 3, -1},
            {0, 1, 3, -1},
            {0, 1, 3, -1},
            {1, -1},
    };

    if (operation_id == UNKNOWN_COMMAND_ID) {
        /* unknown command */
        return FALSE;
    } else if (operation_id >= CLR_ID) {
        /* this commands don't have origin operand */
        return FALSE;
    }
    else if(address_type == -1){
        /* couldn't parse addressing type */
        return FALSE;
    }
    operation_id--; /* operation ids 1-5 match with indexes 0 -4 in the valid_addressing_types array*/

    valids = valid_addressing_types[operation_id];

    for (i = 0; valids[i] != -1; ++i) {
        if(valids[i] == address_type){
            return TRUE;
        }
    }

    return FALSE;
}