# Makefile for the Adafruit I2C 128 x 64 display with dpad and two buttons

CC=gcc
errors=-Wall -Werror
libs =-lgpiod
examples_dir=./examples/
etchsketch_src=./examples/etchsketch.c
cursor_src=./examples/cursor.c
davogl_point_src=./examples/davogl_point.c
src_dir =src
srcs = $(wildcard ./src/*.c)

.PHONY = all clean
all: etchsketch.out cursor.out davogl_point.out

etchsketch.out: $(etchsketch_src) $(srcs)
	$(CC) -o etchsketch.out $(etchsketch_src) $(srcs) $(errors) $(libs)

cursor.out:  $(cursor_src) $(srcs)
	$(CC) -o cursor.out $(cursor_src) $(srcs) $(errors) $(libs)

davogl_point.out:  $(davogl_point_src) $(srcs)
	$(CC) -o davogl_point.out $(davogl_point_src) $(srcs) $(errors) $(libs)

clean:
	rm *.out
