# Makefile for the Adafruit I2C 128 x 64 display with dpad and two buttons

CC=gcc
errors=-Wall -Werror

bin_dir = "bin"

all: bonnet.out

bonnet.out: bonnet.c
	$(CC) $(errors) $< -o $@

clean:
	rm -f bonnet.out
