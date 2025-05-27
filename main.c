#include "main.h"
#include "vertex.h"
#include "matrix.h"
#include "display.h"
#include "bitmap.h"
#include "mesh.h"
#include <time.h>
#include <math.h>
#include <stdio.h>

int main() {
	init_display();
	vertex_t min_y_vert = {{-1.f, -1.f, 0.f, 1.f}, {0.f, 0.f, 0.f, 0.f}};
	vertex_t mid_y_vert = {{0.f, 1.f, 0.f, 1.f}, {0.5f, 1.f, 0.f, 0.f}};
	vertex_t max_y_vert = {{1.f, -1.f, 0.f, 1.f}, {1.f, 0.f, 0.f, 0.f}};

	mesh_t *mesh = create_mesh("monkey1.obj");
	bitmap_t *texture = create_bitmap_file("bricks.png");

	float rot_counter = 0.f;
	unsigned long long start_time, delta_time = 0;
	struct timespec ts;
	while (true) {
		timespec_get(&ts, TIME_UTC);
		start_time = (unsigned long long)ts.tv_sec * 1000000000 + (unsigned long long)ts.tv_nsec;
		if (kbhit()) {
			int c = getch();
			if (c == 'q') {	
				break;
			}
		}
		clear();
		rot_counter += delta_time * 0.000000001f;
		matrix_t translation = init_matrix_translation(0.f, 0.f, 3.f);
		matrix_t rotation = init_matrix_rotation(rot_counter, 0.f, rot_counter);
		matrix_t transform = multiply_matricies(get_projection_matrix(), multiply_matricies(translation, rotation));
		//fill_triangle(vertex_transform(max_y_vert, transform), vertex_transform(mid_y_vert, transform), vertex_transform(min_y_vert, transform), texture); 
		draw_mesh(mesh, transform, texture);
		render();
		res_update();
		timespec_get(&ts, TIME_UTC);
		delta_time = (unsigned long long)ts.tv_sec * 1000000000 + (unsigned long long)ts.tv_nsec - start_time;
	}
	return 0;
}
