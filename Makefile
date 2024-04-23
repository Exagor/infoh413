CC=gcc
CFLAGS=-O3 -Wall

OBJECTS=src/instance.o src/ex1.o src/optimization.o src/timer.o src/utilities.o

.PHONY: clean

all: lop

lop: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o lop

test: lop
	./lop -i instances/N-tiw56r72_250 --best --exchange --cw

clean:
	rm -f src/*~ src/*.o lop
