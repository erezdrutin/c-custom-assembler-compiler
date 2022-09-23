maman14: main.o word.o symbol.o issue.o files.o string_utils.o conversions.o macros.o pre_assembly.o assembler.o first_run.o second_run.o handlers.o validators.o addr_methods.o operators.o
	gcc -ansi -Wall -g -pedantic main.o word.o symbol.o issue.o files.o string_utils.o conversions.o macros.o pre_assembly.o assembler.o first_run.o second_run.o handlers.o validators.o addr_methods.o operators.o -lm -o maman14

main.o: main.c files.h string_utils.h pre_assembly.h assembler.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

addr_methods.o: addr_methods.c addr_methods.h string_utils.h conversions.h validators.h word.h symbol.h operators.h globals.h
	gcc -c -ansi -Wall -pedantic addr_methods.c -o addr_methods.o

assembler.o: assembler.c assembler.h symbol.h globals.h files.h first_run.h second_run.h string_utils.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o

conversions.o: conversions.c conversions.h string_utils.h
	gcc -c -ansi -Wall -pedantic conversions.c -o conversions.o

files.o: files.c files.h word.h symbol.h conversions.h
	gcc -c -ansi -Wall -pedantic files.c -o files.o

first_run.o: first_run.c first_run.h word.h symbol.h handlers.h validators.h string_utils.h
	gcc -c -ansi -Wall -pedantic first_run.c -o first_run.o

handlers.o: handlers.c handlers.h word.h issue.h symbol.h operators.h string_utils.h conversions.h validators.h addr_methods.h
	gcc -c -ansi -Wall -pedantic handlers.c -o handlers.o

issue.o: issue.c issue.h
	gcc -c -ansi -Wall -pedantic issue.c -o issue.o

macros.o: macros.c macros.h string_utils.h
	gcc -c -ansi -Wall -pedantic macros.c -o macros.o

operators.o: operators.c operators.h string_utils.h conversions.h validators.h addr_methods.h word.h issue.h symbol.h globals.h
	gcc -c -ansi -Wall -pedantic operators.c -o operators.o

pre_assembly.o: pre_assembly.c pre_assembly.h macros.h string_utils.h files.h
	gcc -c -ansi -Wall -pedantic pre_assembly.c -o pre_assembly.o

second_run.o: second_run.c second_run.h symbol.h word.h handlers.h validators.h string_utils.h
	gcc -c -ansi -Wall -pedantic second_run.c -o second_run.o

string_utils.o: string_utils.c string_utils.h
	gcc -c -ansi -Wall -pedantic string_utils.c -o string_utils.o

symbol.o: symbol.c symbol.h issue.h globals.h validators.h
	gcc -c -ansi -Wall -pedantic symbol.c -o symbol.o

validators.o: validators.c validators.h globals.h string_utils.h operators.h
	gcc -c -ansi -Wall -pedantic validators.c -o validators.o

word.o: word.c word.h globals.h
	gcc -c -ansi -Wall -pedantic word.c -o word.o

