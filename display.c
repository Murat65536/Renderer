#include "display.h"
#include "matrix.h"
#include "bitmap.h"
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <stdbool.h>
#include <signal.h>
#include <wchar.h>
#include <locale.h>

#define RAND_MAX 255
#define FOV 70.f

int columns, rows;
wchar_t *buffer;
wchar_t *clear_buffer;


struct termios orig_termios;

matrix_t projection;

bitmap_t texture;

bool update = true;

void reset_terminal_mode() {
	printf("\x1b[0m\x1b[2J\x1b[?25h\x1b[8;100;200t");
	tcsetattr(0, TCSANOW, &orig_termios);
	free(buffer);
	free(clear_buffer);
	free(texture.colors);
}

void queue_res_update(int s) {
	update = true;
}

void res_update() {	
	if (update) {
		update = false;
		struct winsize winsz;
		ioctl(0, TIOCGWINSZ, &winsz);
		columns = winsz.ws_col;
		rows = winsz.ws_row;
		projection = init_perspective(FOV * 180.f / 3.14159f, columns / rows * 0.5f, 0.1f, 1000.f);
		free(buffer);
		free(clear_buffer);
		buffer = (wchar_t *)malloc(((columns * rows) * 37 + 5) * sizeof(wchar_t));
		clear_buffer = (wchar_t *)malloc(((columns * rows) * 37 + 5) * sizeof(wchar_t));

		for (int i = 0; i < columns * rows; i++) {
			wcsncpy(buffer + i * 37, L"\x1b[38;2;000;000;000;48;2;000;000;000m▀", 38);
		}
	
		wcsncpy(buffer + columns * rows * 37, L"\x1b[;H", 5);
		wcsncpy(clear_buffer, buffer, rows * columns * 37 + 5);
	}
}

void init_display() {
	setlocale(LC_ALL, "");
	res_update();
	signal(SIGWINCH, queue_res_update);
	
	printf("\x1b[?25l");
	
	texture = create_bitmap(32, 32);
	for (size_t j = 0; j < texture.height; j++) {
		for (size_t i = 0; i < texture.width; i++) {
			bitmap_set_pixel(&texture, i, j, 255, random(), random(), random());
		}
	}
 

	struct termios new_termios;
	tcgetattr(0, &orig_termios);
	memcpy(&new_termios, &orig_termios, sizeof(new_termios));
	atexit(reset_terminal_mode);
	cfmakeraw(&new_termios);
	tcsetattr(0, TCSANOW, &new_termios);
}

matrix_t get_projection_matrix() {
	return projection;
}

bool kbhit() {
	struct timeval tv = {0L, 0L};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv) > 0;
}

int getch() {
	int r;
	unsigned char c;
	if ((r = read(0, &c, sizeof(c))) < 0) {
		return r;
	}
	else {
		return c;
	}
}

void render() {
	printf("%ls", buffer);
}

void clear() {
	wcsncpy(buffer, clear_buffer, rows * columns * 37 + 5);
}

unsigned int get_rgb(unsigned char r, unsigned char g, unsigned char b) {
	return (b << 16) | (g << 8) | r;
}

void plot_point(unsigned int x, unsigned int y, unsigned int color) {
	for (char i = 0; i < 3; i++) {
		for (char digit = 0; digit < 3; digit++) {
			char offset;
			if (y % 2 == 0) {
				offset = 9;
			}
			else {
				offset = 26;
			}
			buffer[(y / 2 * columns + x) * 37 + 4 * i + offset - digit] = 48 + ((color >> 8 * i) & 0xFF) / (int)pow(10, digit) % 10;
		}
	}
}

void draw_scan_line(edge_t *left, edge_t *right, int j) {
	int x_min = (int)ceil(left->x);
	int x_max = (int)ceil(right->x);

	float x_prestep = x_min - left->x;

	float x_dist = right->x - left->x;
	float tex_coord_xx_step = (right->tex_coord_x - left->tex_coord_x) / x_dist;
	float tex_coord_yx_step = (right->tex_coord_y - left->tex_coord_y) / x_dist;
	float one_over_zx_step = (right->one_over_z - left->one_over_z) / x_dist;

	float tex_coord_x = left->tex_coord_x + tex_coord_xx_step * x_prestep;
	float tex_coord_y = left->tex_coord_y + tex_coord_yx_step * x_prestep;
	float one_over_z = left->one_over_z + one_over_zx_step * x_prestep;

	for (int i = x_min; i < x_max; i++) {
		float z = 1.f / one_over_z;
		int src_x = (int)((tex_coord_x * z) * (texture.width - 1) + 0.5f);
		int src_y = (int)((tex_coord_y * z) * (texture.height - 1) + 0.5f);
		plot_point(i, j, texture.colors[src_y * texture.width + src_x]);
	
		one_over_z += one_over_zx_step;
		tex_coord_x += tex_coord_xx_step;
		tex_coord_y += tex_coord_yx_step;
	}
}

void scan_edges(edge_t *a, edge_t *b, bool side) {
	edge_t *left;
	edge_t *right;

	if (side) {
		left = b;
		right = a;
	}
	else {
		left = a;
		right = b;
	}

	int y_start = b->y_start;
	int y_end = b->y_end;

	for (int j = y_start; j < y_end; j++) {
		draw_scan_line(left, right, j);
		edge_step(left);
		edge_step(right);
	}
}

void scan_triangle(vertex_t min_y_vert, vertex_t mid_y_vert, vertex_t max_y_vert, bool side) {
	gradients_t g = create_gradient(min_y_vert, mid_y_vert, max_y_vert);
	edge_t top_to_bottom = create_edge(g, min_y_vert, max_y_vert, 0);
	edge_t top_to_middle = create_edge(g, min_y_vert, mid_y_vert, 0);
	edge_t middle_to_bottom = create_edge(g, mid_y_vert, max_y_vert, 1);
	
	scan_edges(&top_to_bottom, &top_to_middle, side);
	scan_edges(&top_to_bottom, &middle_to_bottom, side);
}


void fill_triangle(vertex_t v1, vertex_t v2, vertex_t v3) {
	matrix_t screen_space = init_screen_space_transform(columns * 0.5f, rows);
	vertex_t min_y_vert = vertex_perspective_divide(vertex_transform(v1, screen_space));
	vertex_t mid_y_vert = vertex_perspective_divide(vertex_transform(v2, screen_space));
	vertex_t max_y_vert = vertex_perspective_divide(vertex_transform(v3, screen_space));

	if (max_y_vert.pos.y < mid_y_vert.pos.y) {
		vertex_t temp = max_y_vert;
		max_y_vert = mid_y_vert;
		mid_y_vert = temp;
	}

	if (mid_y_vert.pos.y < min_y_vert.pos.y) {
		vertex_t temp = mid_y_vert;
		mid_y_vert = min_y_vert;
		min_y_vert = temp;
	}

	if (max_y_vert.pos.y < mid_y_vert.pos.y) {
		vertex_t temp = max_y_vert;
		max_y_vert = mid_y_vert;
		mid_y_vert = temp;
	}

	scan_triangle(min_y_vert, mid_y_vert, max_y_vert, triangle_cross_product(min_y_vert, max_y_vert, mid_y_vert) >= 0);
}
