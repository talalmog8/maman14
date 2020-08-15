#include "assembler.h"

static unsigned int IC; /* stores current IC */
static unsigned int DC; /* stores current DC */

/*
 * Sets current IC to specified integer
 */
void setIC(unsigned int ic){
    IC = ic;
}

/*
 * Returns current IC
 */
unsigned int getIC(){
    return IC;
}

/*
 * Increments current IC with specified integer
 */
void incIC(unsigned  int addition){
    IC += addition;
}

/*
 * Sets current DC to specified integer
 */
void setDC(unsigned int dc){
    DC = dc;
}

/*
 * Returns current DC
 */
unsigned int getDC(){
    return DC;
}

/*
 * Increments current DC with specified integer
 */
void incDC(unsigned  int addition){
    DC += addition;
}