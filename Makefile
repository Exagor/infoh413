CC=gcc
CFLAGS=-O3 -Wall

OBJECTS=src/instance.o src/main.o src/optimization.o src/timer.o src/utilities.o

.PHONY: clean

all: lop

lop: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o lop

test: lop
	./lop -i instances/N-be75oi_150 --best --exchange --random

clean:
	rm -f src/*~ src/*.o lop
