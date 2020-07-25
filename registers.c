#include "assembler.h"

static unsigned int IC;
static unsigned int DC;

void setIC(int ic){
    IC = ic;
}

unsigned int getIC(){
    return IC;
}

void incIC(unsigned  int addition){
    IC += addition;
}

void setDC(int dc){
    DC = dc;
}

unsigned int getDC(){
    return DC;
}

void incDC(unsigned  int addition){
    DC += addition;
}