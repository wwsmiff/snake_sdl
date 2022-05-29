CC = clang++
CFLAGS = -c -std=c++17 -Wall -Wextra

main: main.o
	${CC} main.o -o main -lSDL2

main.o: main.cc
	${CC} ${CFLAGS} main.cc

.PHONY: clean
clean:
	rm -f *.o main

.PHONY: run
run:
	./main
