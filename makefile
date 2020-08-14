build: files.o cmdline.o firstpass.o secondpass.o secondpass_parsers.o registers.o  parsed.o utils.o lable.o lableslist.o guide.o command.o operations_parsers.o main.c assembler.h
	gcc -g main.c files.o cmdline.o firstpass.o secondpass.o secondpass_parsers.o registers.o  parsed.o utils.o lable.o lableslist.o guide.o command.o operations_parsers.o -ansi -pedantic -Wall -o build
files.o: files.c assembler.h
	gcc -c files.c -ansi -pedantic -Wall -o files.o	
cmdline.o: cmdline.c assembler.h
	gcc -c cmdline.c -ansi -pedantic -Wall -o cmdline.o
firstpass.o: firstpass.c assembler.h
	gcc -c firstpass.c -ansi -pedantic -Wall -o firstpass.o
secondpass.o: secondpass.c assembler.h
	gcc -c secondpass.c -ansi -pedantic -Wall -o secondpass.o
secondpass_parsers.o: secondpass_parsers.c assembler.h
	gcc -c secondpass_parsers.c -ansi -pedantic -Wall -o secondpass_parsers.o
registers.o: registers.c assembler.h
	gcc -c registers.c -ansi -pedantic -Wall -o registers.o
parsed.o: parsed.c assembler.h
	gcc -c parsed.c -ansi -pedantic -Wall -o parsed.o
utils.o: utils.c assembler.h
	gcc -c utils.c -ansi -pedantic -Wall -o utils.o
lable.o: lable.c assembler.h
	gcc -c lable.c -ansi -pedantic -Wall -o lable.o
lableslist.o: lableslist.c assembler.h
	gcc -c lableslist.c -ansi -pedantic -Wall -o lableslist.o
guide.o: guide.c assembler.h
	gcc -c guide.c -ansi -pedantic -Wall -o guide.o
command.o: command.c assembler.h
	gcc -c command.c -ansi -pedantic -Wall -o command.o
operations_parsers.o: operations_parsers.c assembler.h
	gcc -c operations_parsers.c -ansi -pedantic -Wall -o operations_parsers.o