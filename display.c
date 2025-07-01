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
#include <fcntl.h>

#define FOV 70.f

int columns, rows;
wchar_t *buffer;
wchar_t *clear_buffer;
float *depth_buffer;

matrix_t projection;

int log_fd;

bool update = true;

void queue_res_update(const int s) {
	update = true;
}

void res_update() {
	if (update) {
		update = false;
		struct winsize winsz;
		ioctl(0, TIOCGWINSZ, &winsz);
		columns = winsz.ws_col;
		rows = winsz.ws_row;
		projection = init_perspective(FOV * 180.f / 3.14159265358979f, columns / rows * 0.5f, 0.1f, 1000.f);
		free(buffer);
		free(clear_buffer);
		buffer = malloc((columns * rows * 37 + 5) * sizeof(wchar_t));
		clear_buffer = malloc((columns * rows * 37 + 5) * sizeof(wchar_t));
		depth_buffer = malloc(columns * rows * 2 * sizeof(float));

		for (int i = 0; i < columns * rows; i++) {
			wcsncpy(clear_buffer + i * 37, L"\x1b[38;2;000;000;000;48;2;000;000;000m▀", 38);
		}
	
		wcsncpy(clear_buffer + columns * rows * 37, L"\x1b[;H", 5);
		clear();
	}
}

void init_display() {
  log_fd = open("renderer.log", O_WRONLY | O_CREAT, S_IRWXU);
	setlocale(LC_ALL, "");
	res_update();
	signal(SIGWINCH, queue_res_update);
	
	printf("\x1b[?25l");
}

void render() {
	fprintf(stdout, "%ls", buffer);
	fflush(stdout);
}

void clear() {
	wcsncpy(buffer, clear_buffer, rows * columns * 37 + 5);
	for (int i = 0; i < columns * rows * 2; i++) {
		depth_buffer[i] = FLT_MAX;
	}
}

void plot_point(const unsigned int x, const unsigned int y, const unsigned int color) {
	for (unsigned char i = 0; i < 3; i++) {
		for (unsigned char digit = 0; digit < 3; digit++) {
			buffer[(y / 2 * columns + x) * 37 + 4 * i - digit + (y % 2 == 0 ? 9 : 26)] = 48 + (color >> 8 * i & 0xff) / _pow10[digit] % 10;
		}
	}
}

void plot_string(const unsigned int x, const unsigned int y, const wchar_t *str, const unsigned int foreground_color, const unsigned int background_color) {
	for (size_t i = 0; i < wcslen(str); i++) {
		plot_character(x + i, y, str[i], foreground_color, background_color);
	}
}

void plot_character(const unsigned int x, const unsigned int y, const wchar_t character, const unsigned int foreground_color, const unsigned int background_color) {
	for (unsigned char i = 0; i < 3; i++) {
		for (unsigned char digit = 0; digit < 3; digit++) {
			const unsigned int position = (y * columns + x) * 37 + 4 * i - digit;
			buffer[position + 9] = 48 + (foreground_color >> 8 * i & 0xff) / _pow10[digit] % 10;
			buffer[position + 26] = 48 + (background_color >> 8 * i & 0xff) / _pow10[digit] % 10;
		}
	}
	buffer[(y * columns + x) * 37 + 36] = character;
}

void draw_scan_line(const gradients_t g, const edge_t *left, const edge_t *right, const int j, const bitmap_t *texture) {
	const int x_min = _ceil(left->x);
	const int x_max = _ceil(right->x);

	const float x_prestep = x_min - left->x;
	float tex_coord_x = left->tex_coord_x + g.tex_coord_xx_step * x_prestep;
	float tex_coord_y = left->tex_coord_y + g.tex_coord_yx_step * x_prestep;
	float one_over_z = left->one_over_z + g.one_over_zx_step * x_prestep;
	float depth = left->depth + g.depth_x_step * x_prestep;
    float light_amt = left->light_amt + g.light_amt_x_step * x_prestep;

	for (int i = x_min; i < x_max; i++) {
		const int index = i + j * columns;
		if (depth < depth_buffer[index]) {
			depth_buffer[index] = depth;
			const float z = 1.f / one_over_z;
			const int src_x = (int)((tex_coord_x * z) * (texture->width - 1) + 0.5f);
			const int src_y = (int)((tex_coord_y * z) * (texture->height - 1) + 0.5f);
			plot_point(i, j, 
              ((unsigned char)(texture->colors[(src_y * texture->width + src_x) * 3 + 2] * light_amt) << 16) |
              ((unsigned char)(texture->colors[(src_y * texture->width + src_x) * 3 + 1] * light_amt) << 8) |
              (unsigned char)(texture->colors[(src_y * texture->width + src_x) * 3] * light_amt));
		}
	
		one_over_z += g.one_over_zx_step;
		tex_coord_x += g.tex_coord_xx_step;
		tex_coord_y += g.tex_coord_yx_step;
    depth += g.depth_x_step;
    light_amt += g.light_amt_x_step;
	}
}

void scan_edges(const gradients_t g, edge_t *a, edge_t *b, const bool side, const bitmap_t *texture) {
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
		draw_scan_line(g, left, right, j, texture);
		edge_step(left);
		edge_step(right);
	}
}

void scan_triangle(const vertex_t min_y_vert, const vertex_t mid_y_vert, const vertex_t max_y_vert, const bool side, const bitmap_t *texture) {
	gradients_t g = create_gradient(min_y_vert, mid_y_vert, max_y_vert);
	edge_t top_to_bottom = create_edge(g,  min_y_vert, max_y_vert, 0);
	edge_t top_to_middle = create_edge(g, min_y_vert, mid_y_vert, 0);
	edge_t middle_to_bottom = create_edge(g, mid_y_vert, max_y_vert, 1);

	scan_edges(g, &top_to_bottom, &top_to_middle, side, texture);
	scan_edges(g, &top_to_bottom, &middle_to_bottom, side, texture);
}

bool clip_polygon(list_t *vertices) {
  list_t *auxiliary_list = create_list(sizeof(vertex_t));
	clip_polygon_component(vertices, 0, false, auxiliary_list);
	list_clear(vertices);
	if (auxiliary_list->length == 0) {
        goto NoClip;
	}
	clip_polygon_component(auxiliary_list, 0, true, vertices);
	list_clear(auxiliary_list);
    if (vertices->length == 0) {
        goto NoClip;
    }

  clip_polygon_component(vertices, 1, false, auxiliary_list);
	list_clear(vertices);
	if (auxiliary_list->length == 0) {
        goto NoClip;
	}
	clip_polygon_component(auxiliary_list, 1, true, vertices);
	list_clear(auxiliary_list);
    if (vertices->length == 0) {
        goto NoClip;
    }

  clip_polygon_component(vertices, 2, false, auxiliary_list);
	list_clear(vertices);
	if (auxiliary_list->length == 0) {
        goto NoClip;
	}
	clip_polygon_component(auxiliary_list, 2, true, vertices);
	list_clear(auxiliary_list);
	if (vertices->length == 0) {
        goto NoClip;
	}
  free_list(auxiliary_list);
  return true;
NoClip:
  free_list(auxiliary_list);
  return false;
}

void clip_polygon_component(list_t *vertices, const char component_index, const bool negative, list_t *result) {
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
  }
	if (negative) {
    previous_component *= -1;
  }
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
    }
	  if (negative) {
      current_component *= -1;
    }
    bool current_inside = current_component <= current_vertex.pos.w;
	  if (current_inside ^ previous_inside) {
      vertex_t v = vertex_lerp(previous_vertex, current_vertex, (previous_vertex.pos.w - previous_component) / ((previous_vertex.pos.w - previous_component) - (current_vertex.pos.w - current_component)));
		  list_add(result, &v);
		}
	  if (current_inside) {
	    list_add(result, &current_vertex);
	  }
    previous_vertex = current_vertex;
    previous_component = current_component;
    previous_inside = current_inside;
  }
}

void draw_triangle(vertex_t v1, vertex_t v2, vertex_t v3, const bitmap_t *texture) {
	const bool v1_inside = inside_view_frustum(v1);
	const bool v2_inside = inside_view_frustum(v2);
	const bool v3_inside = inside_view_frustum(v3);

	if (v1_inside && v2_inside && v3_inside) {
		fill_triangle(v1, v2, v3, texture);
		return;
	}
	if (!v1_inside && !v2_inside && !v3_inside) {
		return;
	}

	list_t *vertices = create_list(sizeof(vertex_t));

	list_add(vertices, &v1);
	list_add(vertices, &v2);
	list_add(vertices, &v3);

	if (clip_polygon(vertices)) {
		vertex_t initial_vertex = *(vertex_t *)vertices->array[0];
		for (size_t i = 1; i < vertices->length - 1; i++) {
			fill_triangle(initial_vertex, *(vertex_t *)vertices->array[i], *(vertex_t *)vertices->array[i + 1], texture);
		}
	}
}

void fill_triangle(const vertex_t v1, const vertex_t v2, const vertex_t v3, const bitmap_t *texture) {
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

void draw_mesh(const mesh_t *mesh, const matrix_t view_projection) {
  const matrix_t matrix_view_proj = multiply_matrices(view_projection, mesh->projection);
	for (size_t i = 0; i < mesh->indices->length; i+=3) {
		draw_triangle(
				vertex_transform_normal(*(vertex_t *)mesh->vertices->array[*(size_t *)mesh->indices->array[i]], matrix_view_proj, mesh->projection),
				vertex_transform_normal(*(vertex_t *)mesh->vertices->array[*(size_t *)mesh->indices->array[i + 1]], matrix_view_proj, mesh->projection),
				vertex_transform_normal(*(vertex_t *)mesh->vertices->array[*(size_t *)mesh->indices->array[i + 2]], matrix_view_proj, mesh->projection),
				mesh->texture);
	}
}
