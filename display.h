#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "vertex.h"
#include "edge.h"
#include "matrix.h"
#include "mesh.h"
#include "bitmap.h"
#include "list.h"
#include <stdbool.h>

extern matrix_t projection;

void queue_res_update(const int s);

void res_update();

void init_display();

void render();

void clear();

void plot_point(const unsigned int x, const unsigned int y, const unsigned int color);

void plot_string(const unsigned int x, const unsigned int y, const wchar_t *str, const unsigned int foreground_color, const unsigned int background_color);

void plot_character(const unsigned int x, const unsigned int y, const wchar_t character, const unsigned int foreground_color, const unsigned int background_color);

void draw_scan_line(const edge_t *left, const edge_t *right, const int j, const bitmap_t *texture);

void scan_edges(edge_t *a, edge_t *b, const bool side, const bitmap_t *texture);

void scan_triangle(const vertex_t min_y_vert, const vertex_t mid_y_vert, const vertex_t max_y_vert, const bool side, const bitmap_t *texture);

bool clip_polygon_axis(list_t *vertices, list_t *auxillary_list, const char component_index);

void clip_polygon_component(list_t *vertices, const char component_index, const bool negative, list_t *result);

void draw_triangle(vertex_t v1, vertex_t v2, vertex_t v3, const bitmap_t *texture);

void fill_triangle(const vertex_t v1, const vertex_t v2, const vertex_t v3, const bitmap_t *texture);

void draw_mesh(const mesh_t *mesh, const matrix_t transform, const bitmap_t *texture);

#endif
