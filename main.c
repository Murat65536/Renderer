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
		matrix_t translation = init_matrix_translation(0.f, 0.f, 1.f);
		matrix_t rotation = init_matrix_rotation(rot_counter, 0.f, rot_counter);
		matrix_t transform = multiply_matricies(get_projection_matrix(), multiply_matricies(translation, rotation));
		draw_mesh(mesh, transform, texture);
		char fps[14];
		snprintf(fps, 14, "FPS: %f", 1000000000.f / delta_time);
		wchar_t str[14];
		for (int i = 0; i < 14; i++) {
			str[i] = (wchar_t)fps[i];
		}
		plot_string(10, 10, str, (0 << 16) | (0 << 8) | 255, (0 << 16) | (255 << 8) | 0);
		render();
		res_update();
		timespec_get(&ts, TIME_UTC);
		delta_time = (unsigned long long)ts.tv_sec * 1000000000 + (unsigned long long)ts.tv_nsec - start_time;
	}
	return 0;
}
