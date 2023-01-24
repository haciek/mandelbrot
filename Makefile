CC=gcc
# CFLAGS=-Wall -g
# LIBS=-lm -lSDL2_image
CFLAGS = `sdl2-config --libs --cflags` -ggdb3 -O4 --std=c99 -Wall -lSDL2_image -lm
main: main.c
	$(CC) $(CFLAGS) -o $@ $^
