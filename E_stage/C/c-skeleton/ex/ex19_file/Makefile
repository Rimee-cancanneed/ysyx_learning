CFLAGS=-Wall -g
all: ex19
ex19: ex19.o object.o
	gcc $(CFLAGS) -o ex19 ex19.o object.o
ex19.o: ex19.c ex19.h object.h
	gcc $(CFLAGS) -c ex19.c ex19.h object.h
object.o: object.c object.h
	gcc $(CFLAGS) -c object.c
clean:
	rm -f ex19