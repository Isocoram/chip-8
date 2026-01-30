CC = clang
CFLAGS = -Wall -Wextra -O2 $(shell pkg-config --cflags sdl2) -Iroms -Isrc
LDFLAGS = $(shell pkg-config --libs sdl2)

chip8:
	$(CC) src/*.c roms/*.c -o bin/chip8 $(CFLAGS) $(LDFLAGS)
