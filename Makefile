CC=gcc
CFLAGS+= -Wall -Wextra -std=c11

OBJECTS:= main.o

ball: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

.PHONY: clean run

clean:
	rm -f ball $(OBJECTS)

run:
	make ball
	./ball
