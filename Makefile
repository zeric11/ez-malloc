CC = gcc
CFLAGS = -O3 -Wall

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,build/%.o,$(SRCS))
PROGRAM_NAME = ez_malloc_test

$(PROGRAM_NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o build/$(PROGRAM_NAME)

build/%.o: src/%.c # Compile but do not link source files
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*