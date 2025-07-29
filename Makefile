# Makefile for the Adafruit I2C 128 x 64 display with dpad and two buttons

CC=gcc
errors=-Wall -Werror
libs=-lgpiod
examples_dir=./examples/
etchsketch_src=./examples/etchsketch.c
cursor_src=./examples/cursor.c
pong_src=./examples/pong.c
davogl_point_src=./examples/davogl_point.c
src_dir=src
tests_gl_dir=tests_gl
srcs=$(wildcard ./src/*.c)
uic_srcs=$(wildcard ./src/uic/*.c)
tests=$(wildcard ./tests_gl/*.c)

.PHONY = all clean test
all: etchsketch.out cursor.out davogl_point.out

etchsketch.out: $(etchsketch_src) $(srcs)
	$(CC) -o etchsketch.out $(etchsketch_src) $(srcs) $(errors) $(libs)

cursor.out:  $(cursor_src) $(srcs)
	$(CC) -o cursor.out $(cursor_src) $(srcs) $(errors) $(libs)

davogl_point.out:  $(davogl_point_src) $(srcs)
	$(CC) -o davogl_point.out $(davogl_point_src) $(srcs) $(errors) $(libs)

pong.out:  $(pong_src) $(srcs)
	$(CC) -o pong.out $(pong_src) $(srcs) $(errors) $(libs) -g

# tests:
set_pixel.out: $(srcs) ./tests_gl/set_pixel.c
	$(CC) -o set_pixel.out ./tests_gl/set_pixel.c $(srcs) $(errors) $(libs)
draw_line.out: $(srcs) ./tests_gl/draw_line.c
	$(CC) -o draw_line.out ./tests_gl/draw_line.c $(srcs) $(errors) $(libs) -lm
draw_circle.out: $(srcs) ./tests_gl/draw_circle.c
	$(CC) -o draw_circle.out ./tests_gl/draw_circle.c $(srcs) $(errors) $(libs) -lm
draw_rect.out: $(srcs) ./tests_gl/draw_rect.c
	$(CC) -o draw_rect.out ./tests_gl/draw_rect.c $(srcs) $(errors) $(libs) -lm
draw_arc.out: $(srcs) ./tests_gl/draw_arc.c
	$(CC) -o draw_arc.out ./tests_gl/draw_arc.c $(srcs) $(errors) $(libs) -lm
draw_triangle.out: $(srcs) ./tests_gl/draw_triangle.c
	$(CC) -o draw_triangle.out ./tests_gl/draw_triangle.c $(srcs) $(errors) $(libs) -lm
draw_ellipse.out: $(srcs) ./tests_gl/draw_ellipse.c
	$(CC) -o draw_ellipse.out ./tests_gl/draw_ellipse.c $(srcs) $(errors) $(libs) -lm


progress_bar.out: $(srcs) $(uic_srcs) ./tests_gl/uic_progress_bar.c
	$(CC) -o progress_bar.out ./tests_gl/uic_progress_bar.c $(srcs) $(uic_srcs) $(errors) $(libs) -lm -g
clean:
	rm *.out
