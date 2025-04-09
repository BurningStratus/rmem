CC=gcc

rmem: main.c
	$(CC) -g -o rmem -Wno-unused -Wall main.c
