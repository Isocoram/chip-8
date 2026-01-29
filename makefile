CC = clang
CFLAGS = -Wall -Wextra -O2 $(shell pkg-config --cflags sdl2)
LDFLAGS = $(shell pkg-config --libs sdl2)

chip8:
	$(CC) src/*.c -o chip8 $(CFLAGS) $(LDFLAGS)
