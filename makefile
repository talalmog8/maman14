build: files.o cmdline.o main.c assembler.h
	gcc -g main.c files.o cmdline.o -ansi -pedantic -Wall -o build
files.o: files.c assembler.h
	gcc -c files.c -ansi -pedantic -Wall -o files.o	
cmdline.o: cmdline.c assembler.h
	gcc -c cmdline.c -ansi -pedantic -Wall -o cmdline.o