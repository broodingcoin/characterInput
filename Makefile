all: mc2

mc1: mc2.o
	gcc -g mc2.o -o mc2

mc1.o: mc2.c
	gcc -g -c mc2.c mc2.h

clean:
	rm -f mc2
	rm -f *.o
