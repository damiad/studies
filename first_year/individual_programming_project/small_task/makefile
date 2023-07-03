CC = gcc
CFLAGS = -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2

objects = labyrinth.o operations.o read.o bfs.o
labyrinth: $(objects)
all: labyrinth

$(objects): %.o: %.c

clean:
	rm -f *.o labyrinth