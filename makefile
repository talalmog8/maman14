build.o: parsed.o cmdline.o files.o firstpass.o secondpass.o labelslist.o entries_output.o externals_output.o externals.o logger.o main.c assembler.h labelslist.h
	gcc -g main.c externals_output.o externals.o entries_output.o parsed.o cmdline.o files.o firstpass.o secondpass.o labelslist.o secondpass_parsers.o registers.o utils.o label.o guide.o command.o firstpass_parsers.o addressing_types.o logger.o -ansi -pedantic -Wall -o build.o
files.o: files.c assembler.h
	gcc -c files.c -ansi -pedantic -Wall -o files.o	
cmdline.o: cmdline.c assembler.h
	gcc -c cmdline.c -ansi -pedantic -Wall -o cmdline.o
firstpass.o: firstpass.c files.o registers.o utils.o label.o guide.o labelslist.o command.o assembler.h labelslist.h
	gcc -c firstpass.c -ansi -pedantic -Wall -o firstpass.o
secondpass.o: secondpass.c files.o registers.o utils.o label.o guide.o labelslist.o command.o assembler.h labelslist.h
	gcc -c secondpass.c -ansi -pedantic -Wall -o secondpass.o
secondpass_parsers.o: secondpass_parsers.c registers.o parsed.o label.o labelslist.o assembler.h command_template.h labelslist.h firstpass_parsers.h
	gcc -c secondpass_parsers.c -ansi -pedantic -Wall -o secondpass_parsers.o
registers.o: registers.c assembler.h
	gcc -c registers.c -ansi -pedantic -Wall -o registers.o
parsed.o: parsed.c files.o assembler.h
	gcc -c parsed.c -ansi -pedantic -Wall -o parsed.o
utils.o: utils.c assembler.h
	gcc -c utils.c -ansi -pedantic -Wall -o utils.o
label.o: label.c assembler.h
	gcc -c label.c -ansi -pedantic -Wall -o label.o
labelslist.o: labelslist.c assembler.h labelslist.h
	gcc -c labelslist.c -ansi -pedantic -Wall -o labelslist.o
guide.o: guide.c utils.o parsed.o registers.o assembler.h
	gcc -c guide.c -ansi -pedantic -Wall -o guide.o
command.o: command.c utils.o firstpass_parsers.o secondpass_parsers.o label.o assembler.h firstpass_parsers.h
	gcc -c command.c -ansi -pedantic -Wall -o command.o
firstpass_parsers.o: firstpass_parsers.c parsed.o registers.o label.o utils.o addressing_types.o assembler.h command_template.h firstpass_parsers.h
	gcc -c firstpass_parsers.c -ansi -pedantic -Wall -o firstpass_parsers.o
entries_output.o: entries_output.c files.o assembler.h labelslist.h
	gcc -c entries_output.c -ansi -pedantic -Wall -o entries_output.o
externals.o: externals.c assembler.h
	gcc -c externals.c -ansi -pedantic -Wall -o externals.o
externals_output.o: externals_output.c
	gcc -c  externals_output.c -ansi -pedantic -Wall -o externals_output.o
addressing_types.o: addressing_types.c assembler.h
	gcc -c  addressing_types.c -ansi -pedantic -Wall -o addressing_types.o
logger.o: logger.c assembler.h
	gcc -c  logger.c -ansi -pedantic -Wall -o logger.o