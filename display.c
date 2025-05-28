#include "display.h"
#include "matrix.h"
#include "bitmap.h"
#include "mesh.h"
#include "list.h"
#include "math.h"
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
#include <float.h>

#define FOV 70.f

int columns, rows;
wchar_t *buffer;
wchar_t *clear_buffer;
float *depth_buffer;

struct termios orig_termios;

matrix_t projection;


bool update = true;

void reset_terminal_mode() {
	printf("\x1b[0m\x1b[2J\x1b[?25h\x1b[8;100;200t");
	tcsetattr(0, TCSANOW, &orig_termios);
	free(buffer);
	free(clear_buffer);
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
		buffer = (wchar_t *)malloc((columns * rows * 37 + 5) * sizeof(wchar_t));
		clear_buffer = (wchar_t *)malloc((columns * rows * 37 + 5) * sizeof(wchar_t));
		depth_buffer = (float *)malloc(columns * rows * 2 * sizeof(float));

		for (int i = 0; i < columns * rows; i++) {
			wcsncpy(clear_buffer + i * 37, L"\x1b[38;2;000;000;000;48;2;000;000;000m▀", 38);
		}
	
		wcsncpy(clear_buffer + columns * rows * 37, L"\x1b[;H", 5);
		clear();
	}
}

void init_display() {
	setlocale(LC_ALL, "");
	res_update();
	signal(SIGWINCH, queue_res_update);
	
	printf("\x1b[?25l");
	
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
	fflush(stdout);
}

void clear() {
	wcsncpy(buffer, clear_buffer, rows * columns * 37 + 5);
	for (int i = 0; i < columns * rows * 2; i++) {
		depth_buffer[i] = FLT_MAX;
	}
}

void plot_point(unsigned int x, unsigned int y, unsigned int color) {
	for (unsigned char i = 0; i < 3; i++) {
		for (unsigned char digit = 0; digit < 3; digit++) {
			char offset;
			if (y % 2 == 0) {
				offset = 9;
			}
			else {
				offset = 26;
			}
			buffer[(y / 2 * columns + x) * 37 + 4 * i - digit + offset] = 48 + (color >> 8 * i & 0xff) / pow10_[digit] % 10;
		}
	}
}

void plot_string(unsigned int x, unsigned int y, wchar_t *str, unsigned int foreground_color, unsigned int background_color) {
	for (size_t i = 0; i < wcslen(str); i++) {
		plot_character(x + i, y, str[i], foreground_color, background_color);
	}
}

void plot_character(unsigned int x, unsigned int y, wchar_t character, unsigned int foreground_color, unsigned int background_color) {
	for (unsigned char i = 0; i < 3; i++) {
		for (unsigned char digit = 0; digit < 3; digit++) {
			unsigned int position = (y * columns + x) * 37 + 4 * i - digit;
			buffer[position + 9] = 48 + (foreground_color >> 8 * i & 0xff) / pow10_[digit] % 10;
			buffer[position + 26] = 48 + (background_color >> 8 * i & 0xff) / pow10_[digit] % 10;
		}
	}
	buffer[(y * columns + x) * 37 + 36] = character;
}

void draw_scan_line(edge_t *left, edge_t *right, int j, bitmap_t *texture) {
	int x_min = (int)ceil(left->x);
	int x_max = (int)ceil(right->x);

	float x_prestep = x_min - left->x;

	float x_dist = right->x - left->x;
	float tex_coord_xx_step = (right->tex_coord_x - left->tex_coord_x) / x_dist;
	float tex_coord_yx_step = (right->tex_coord_y - left->tex_coord_y) / x_dist;
	float one_over_zx_step = (right->one_over_z - left->one_over_z) / x_dist;
	float depth_x_step = (right->depth - left->depth) / x_dist;

	float tex_coord_x = left->tex_coord_x + tex_coord_xx_step * x_prestep;
	float tex_coord_y = left->tex_coord_y + tex_coord_yx_step * x_prestep;
	float one_over_z = left->one_over_z + one_over_zx_step * x_prestep;
	float depth = left->depth + depth_x_step * x_prestep;

	for (int i = x_min; i < x_max; i++) {
		int index = i + j * columns;
		if (depth < depth_buffer[index]) {
			depth_buffer[index] = depth;
			float z = 1.f / one_over_z;
			unsigned int src_x = (unsigned int)fmax((tex_coord_x * z) * (texture->width - 1) + 0.5f, 0.f);
			unsigned int src_y = (unsigned int)fmax((tex_coord_y * z) * (texture->height - 1) + 0.5f, 0.f);
			plot_point(i, j, (texture->colors[(src_y * texture->width + src_x) * 3] << 16) | (texture->colors[(src_y * texture->width + src_x) * 3 + 1] << 8) | texture->colors[(src_y * texture->width + src_x) * 3 + 2]);
		}
	
		one_over_z += one_over_zx_step;
		tex_coord_x += tex_coord_xx_step;
		tex_coord_y += tex_coord_yx_step;
	}
}

void scan_edges(edge_t *a, edge_t *b, bool side, bitmap_t *texture) {
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
		draw_scan_line(left, right, j, texture);
		edge_step(left);
		edge_step(right);
	}
}

void scan_triangle(vertex_t min_y_vert, vertex_t mid_y_vert, vertex_t max_y_vert, bool side, bitmap_t *texture) {
	gradients_t g = create_gradient(min_y_vert, mid_y_vert, max_y_vert);
	edge_t top_to_bottom = create_edge(g,  min_y_vert, max_y_vert, 0);
	edge_t top_to_middle = create_edge(g, min_y_vert, mid_y_vert, 0);
	edge_t middle_to_bottom = create_edge(g, mid_y_vert, max_y_vert, 1);

	scan_edges(&top_to_bottom, &top_to_middle, side, texture);
	scan_edges(&top_to_bottom, &middle_to_bottom, side, texture);
}

bool clip_polygon_axis(list_t *vertices, list_t *auxillary_list, char component_index) {
	clip_polygon_component(vertices, component_index, 1.f, auxillary_list);
	list_clear(vertices);
	if (auxillary_list->length == 0) {
		return false;
	}
	clip_polygon_component(auxillary_list, component_index, -1.f, vertices);
	list_clear(auxillary_list);
	return !vertices->length == 0;
}

void clip_polygon_component(list_t *vertices, char component_index, float component_factor, list_t *result) {
	vertex_t previous_vertex = *(vertex_t *)vertices->array[vertices->length - 1];
	float previous_component;
	switch (component_index) {
		case 0:
			previous_component = previous_vertex.pos.x;
			break;
		case 1:
			previous_component = previous_vertex.pos.y;
			break;
		case 2:
			previous_component = previous_vertex.pos.z;
			break;
		case 3:
			previous_component = previous_vertex.pos.w;
			break;
		default:
			exit(1);
	}
	previous_component *= component_factor;
	bool previous_inside = previous_component <= previous_vertex.pos.w;

	for (size_t i = 0; i < vertices->length; i++) {
		vertex_t current_vertex = *(vertex_t *)vertices->array[i];
		float current_component;
		switch (component_index) {
			case 0:
				current_component = current_vertex.pos.x;
				break;
			case 1:
				current_component = current_vertex.pos.y;
				break;
			case 2:
				current_component = current_vertex.pos.z;
				break;
			case 3:
				current_component = current_vertex.pos.w;
				break;
			default:
				exit(1);
		}
		current_component *= component_factor;
		bool current_inside = current_component <= current_vertex.pos.w;

		if (current_inside ^ previous_inside) {
			float lerp_factor = (previous_vertex.pos.w - previous_component) / ((previous_vertex.pos.w - previous_component) - (current_vertex.pos.w - current_component));
			list_add(result, malloc_vertex(vertex_lerp(previous_vertex, current_vertex, lerp_factor)));
		}

		if (current_inside) {
			list_add(result, vertices->array[i]);
		}

		previous_vertex = *(vertex_t *)vertices->array[i];
		previous_component = current_component;
		previous_inside = current_inside;
	}
}

void draw_triangle(vertex_t v1, vertex_t v2, vertex_t v3, bitmap_t *texture) {
	bool v1_inside = inside_view_frustum(v1);
	bool v2_inside = inside_view_frustum(v2);
	bool v3_inside = inside_view_frustum(v3);

	if (v1_inside && v2_inside && v3_inside) {
		fill_triangle(v1, v2, v3, texture);
		return;
	}
	if (!v1_inside && !v2_inside && !v3_inside) {
		return;
	}

	list_t *vertices = create_list(sizeof(vertex_t));
	list_t *auxillary_list = create_list(sizeof(vertex_t));

	list_add(vertices, &v1);
	list_add(vertices, &v2);
	list_add(vertices, &v3);

	if (clip_polygon_axis(vertices, auxillary_list, 0) &&
			clip_polygon_axis(vertices, auxillary_list, 1) && 
			clip_polygon_axis(vertices, auxillary_list, 2)) {
		vertex_t initial_vertex = *(vertex_t *)vertices->array[0];
		for (size_t i = 1; i < vertices->length - 1; i++) {
			fill_triangle(initial_vertex, *(vertex_t *)vertices->array[i], *(vertex_t *)vertices->array[i + 1], texture);
		}
	}
}

void fill_triangle(vertex_t v1, vertex_t v2, vertex_t v3, bitmap_t *texture) {
	matrix_t screen_space = init_screen_space_transform(columns * 0.5f, rows);
	vertex_t min_y_vert = vertex_perspective_divide(vertex_transform(v1, screen_space));
	vertex_t mid_y_vert = vertex_perspective_divide(vertex_transform(v2, screen_space));
	vertex_t max_y_vert = vertex_perspective_divide(vertex_transform(v3, screen_space));

	if (triangle_cross_product(min_y_vert, max_y_vert, mid_y_vert) >= 0) {
		return;
	}

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

	scan_triangle(min_y_vert, mid_y_vert, max_y_vert, triangle_cross_product(min_y_vert, max_y_vert, mid_y_vert) >= 0, texture);
}

void draw_mesh(mesh_t *mesh, matrix_t transform, bitmap_t *texture) {
	for (size_t i = 0; i < mesh->indices->length; i+=3) {
		draw_triangle(
				vertex_transform(*(vertex_t *)mesh->vertices->array[*(size_t *)mesh->indices->array[i]], transform),
				vertex_transform(*(vertex_t *)mesh->vertices->array[*(size_t *)mesh->indices->array[i + 1]], transform),
				vertex_transform(*(vertex_t *)mesh->vertices->array[*(size_t *)mesh->indices->array[i + 2]], transform),
				texture);
	}
}
