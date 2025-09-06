# Makefile for the Adafruit I2C 128 x 64 display with dpad and two buttons

CC=gcc
errors=-Wall -Werror
libs=-lgpiod -lm
src_dir=src
tests_gl_dir=tests_gl
srcs=$(wildcard ./src/*.c)
uic_srcs=$(wildcard ./src/uic/*.c)
fonts=$(wildcard ./fonts/*.h)
tests=$(wildcard ./tests_gl/*.c)
test_dir=tests_gl/

.PHONY = all clean tests_gl
all: tests

# tests:

tests_gl: $(patsubst %.c, %.out, $(tests))

$(test_dir)set_pixel.out: $(srcs) ./tests_gl/set_pixel.c
	$(CC) -o set_pixel.out ./tests_gl/set_pixel.c $(srcs) $(errors) $(libs) -g

$(test_dir)draw_line.out: $(srcs) ./tests_gl/draw_line.c
	$(CC) -o draw_line.out ./tests_gl/draw_line.c $(srcs) $(errors) $(libs)

$(test_dir)draw_circle.out: $(srcs) ./tests_gl/draw_circle.c
	$(CC) -o draw_circle.out ./tests_gl/draw_circle.c $(srcs) $(errors) $(libs)

$(test_dir)draw_rect.out: $(srcs) ./tests_gl/draw_rect.c
	$(CC) -o draw_rect.out ./tests_gl/draw_rect.c $(srcs) $(errors) $(libs)

$(test_dir)draw_arc.out: $(srcs) ./tests_gl/draw_arc.c
	$(CC) -o draw_arc.out ./tests_gl/draw_arc.c $(srcs) $(errors) $(libs)

$(test_dir)draw_triangle.out: $(srcs) ./tests_gl/draw_triangle.c
	$(CC) -o draw_triangle.out ./tests_gl/draw_triangle.c $(srcs) $(errors) $(libs)

$(test_dir)draw_ellipse.out: $(srcs) ./tests_gl/draw_ellipse.c
	$(CC) -o draw_ellipse.out ./tests_gl/draw_ellipse.c $(srcs) $(errors) $(libs)

$(test_dir)uic_progress_bar.out: $(srcs) $(uic_srcs) ./tests_gl/uic_progress_bar.c $(fonts)
	$(CC) -o progress_bar.out ./tests_gl/uic_progress_bar.c \
		$(srcs) $(uic_srcs) $(fonts) $(errors) $(libs) -g

$(test_dir)draw_text.out: $(srcs) $(fonts) ./tests_gl/uic_progress_bar.c
	$(CC) -o draw_text.out ./tests_gl/draw_text.c $(srcs) $(fonts) $(errors) $(libs) -g

$(test_dir)uic_segment16.out: $(srcs) $(uic_srcs) ./tests_gl/uic_segment16.c
	$(CC) -o uic_segment16.out ./tests_gl/uic_segment16.c $(srcs) $(uic_srcs) \
	$(fonts) $(errors) $(libs) -g



gpio_test.out: gpio_test.c $(srcs)
	$(CC) -o gpio_test.out ./gpio_test.c $(srcs) $(errors) $(libs) -g
clean:
	rm *.out
