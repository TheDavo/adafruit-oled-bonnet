# Makefile for the Adafruit I2C 128 x 64 display with dpad and two buttons

CC=gcc
errors=-Wall -Werror
libs =-lgpiod
src=bonnet.c
output=bonnet.out
etchsketch_src=./examples/etchsketch.c
cursor_src=./examples/cursor.c
bin_dir = "bin"

.PHONY = all
all:
	@echo "use 'make etchsketch' or other example files to create a game"

etchsketch: bonnet.c bonnet.o
	$(CC) -o etchsketch.out $(etchsketch_src) $(src) $(errors) $(libs)

cursor: bonnet.c bonnet.o
	$(CC) -o cursor.out $(cursor_src) $(src) $(errors) $(libs)

clean:
	rm -f bonnet.out
