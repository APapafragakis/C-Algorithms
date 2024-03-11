CC = gcc
CFLAGS = -Wall -ansi -pedantic -g

translate:
	$(CC) $(CFLAGS) src/main.c -o $@

clean:
	-rm -f translate

test:
	./translate < ./tests/test1.7 > test.1
	./translate < ./tests/test2.7 > test.2
