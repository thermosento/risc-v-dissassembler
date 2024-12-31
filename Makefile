CC = clang-15
CFLAGS = -Wall -g3 -gdwarf-4

all: disas

disas: disas.o loader.o decoder.o
	$(CC) $(CFLAGS) -o disas disas.o loader.o decoder.o

disas.o: disas.c loader.h decoder.h
	$(CC) $(CFLAGS) -c disas.c

loader.o: loader.c loader.h
	$(CC) $(CFLAGS) -c loader.c

decoder.o: decoder.c decoder.h
	$(CC) $(CFLAGS) -c decoder.c

clean:
	rm -f *.o disas