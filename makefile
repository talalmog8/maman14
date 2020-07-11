build: lable.o utils.o commandparser.o parse.o files.o cmdline.o main.c assembler.h
	gcc -g main.c lable.o utils.o commandparser.o  parse.o files.o cmdline.o -ansi -pedantic -Wall -o build
files.o: files.c assembler.h
	gcc -c files.c -ansi -pedantic -Wall -o files.o	
cmdline.o: cmdline.c assembler.h
	gcc -c cmdline.c -ansi -pedantic -Wall -o cmdline.o
parse.o: parse.c assembler.h
	gcc -c parse.c -ansi -pedantic -Wall -o parse.o
commandparser.o: commandparser.c assembler.h
	gcc -c commandparser.c -ansi -pedantic -Wall -o commandparser.o
utils.o: utils.c assembler.h
	gcc -c utils.c -ansi -pedantic -Wall -o utils.o
lable.o: lable.c assembler.h
	gcc -c lable.c -ansi -pedantic -Wall -o lable.o
