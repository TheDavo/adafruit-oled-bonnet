# Makefile for the Adafruit I2C 128 x 64 display with dpad and two buttons

CC=gcc
errors=-Wall -Werror
libs =-lgpiod
src=bonnet.c
output=bonnet.out

bin_dir = "bin"

all:
	$(CC) $(src) $(errors) $(libs) -o $(output)

clean:
	rm -f bonnet.out
