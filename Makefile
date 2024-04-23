CC=gcc
CFLAGS=-O3 -Wall

OBJECTS=src/instance.o src/ex1.o src/optimization.o src/timer.o src/utilities.o
OBJECTS2=src/instance.o src/ex2.o src/optimization.o src/timer.o src/utilities.o

.PHONY: clean

all: lop lop2

lop: $(OBJECTS) $(OBJECTS2)
	$(CC) $(CFLAGS) $(OBJECTS) -o lop
	$(CC) $(CFLAGS) $(OBJECTS2) -o lop2

test: lop
	./lop -i instances/N-tiw56r72_250 --best --exchange --cw

clean:
	rm -f src/*~ src/*.o lop lop2
