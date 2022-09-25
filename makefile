

all: shell.o parser.o Shell_commands.o def.o
	gcc -o shell.sh shell.o parser.o Shell_commands.o def.o

shell.o: shell.c parser.h def.h
	gcc -c shell.c

parser.o: parser.c parser.h def.h

Shell_commands.o: Shell_commands.c Shell_commands.h def.h
	gcc -c Shell_commands.c

def.o: def.c def.h
	gcc -c def.c

clean:
	rm *.o