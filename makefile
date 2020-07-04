build: parse.o files.o cmdline.o main.c assembler.h
	gcc -g main.c  parse.o files.o cmdline.o -ansi -pedantic -Wall -o build
files.o: files.c assembler.h
	gcc -c files.c -ansi -pedantic -Wall -o files.o	
cmdline.o: cmdline.c assembler.h
	gcc -c cmdline.c -ansi -pedantic -Wall -o cmdline.o
parse.o: parse.c assembler.h
	gcc -c parse.c -ansi -pedantic -Wall -o parse.o
