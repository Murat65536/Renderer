#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "vertex.h"
#include "edge.h"
#include "matrix.h"
#include "mesh.h"
#include "bitmap.h"
#include "list.h"
#include <stdbool.h>

void reset_terminal_mode();

void queue_res_update(int s);

void res_update();

void init_display();

matrix_t get_projection_matrix();

bool kbhit();

int getch();

void render();

void clear();

void plot_point(unsigned int x, unsigned int y, unsigned int color);

void plot_string(unsigned int x, unsigned int y, wchar_t *str, unsigned int foreground_color, unsigned int background_color);

void plot_character(unsigned int x, unsigned int y, wchar_t character, unsigned int foreground_color, unsigned int background_color);

void draw_scan_line(edge_t *left, edge_t *right, int j, bitmap_t *texture);

void scan_edges(edge_t *a, edge_t *b, bool side, bitmap_t *texture);

void scan_triangle(vertex_t min_y_vert, vertex_t mid_y_vert, vertex_t max_y_vert, bool side, bitmap_t *texture);

bool clip_polygon_axis(list_t *vertices, list_t *auxillary_list, char component_index);

void clip_polygon_component(list_t *vertices, char component_index, float component_factor, list_t *result);

void draw_triangle(vertex_t v1, vertex_t v2, vertex_t v3, bitmap_t *texture);

void fill_triangle(vertex_t v1, vertex_t v2, vertex_t v3, bitmap_t *texture);

void draw_mesh(mesh_t *mesh, matrix_t transform, bitmap_t *texture);

#endif
