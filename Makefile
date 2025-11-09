CC = gcc
CFLAGS = -O3 -Wall -g

ez_malloc_test:
	$(CC) $(CFLAGS) src/*.c -o ez_malloc_test

clean:
	rm -f ez_malloc_test