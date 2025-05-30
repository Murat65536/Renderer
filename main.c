#include "main.h"
#include "vertex.h"
#include "matrix.h"
#include "display.h"
#include "bitmap.h"
#include "mesh.h"
#include "camera.h"
#include "input.h"
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

int main() {
	init_display();

	mesh_t *mesh = create_mesh("monkey1.obj");
	bitmap_t *texture = create_bitmap_file("bricks.png");
	mesh_t *terrain = create_mesh("terrain2.obj");
	bitmap_t *texture_terrain = create_bitmap_file("bricks2.png");
	float rot_counter = 0.f;
	camera_t *camera = malloc(sizeof(camera_t));
	camera->transform = (transform_t) {{0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}, {1.f, 1.f, 1.f, 1.f}};
	unsigned long long start_time, end_time = 0;
	float delta_time = 1.f;
	input_init();
	struct timespec ts;
	while (true) {
		timespec_get(&ts, TIME_UTC);
		start_time = (unsigned long long)ts.tv_sec * 1000000000 + (unsigned long long)ts.tv_nsec;
		camera->projection = get_projection_matrix();
		if (kbhit() && getch(camera, delta_time) == -1) {
			break;
		}
		clear();
		//rot_counter += delta_time;
		matrix_t translation = init_matrix_translation(0.f, 0.f, 1.f);
		matrix_t translation_terrain = init_matrix_translation(0.f, -3.f, 0.f);
		matrix_t rotation = init_matrix_rotation(rot_counter, 0.f, rot_counter);
		matrix_t transform = multiply_matrices(get_view_projection(camera), multiply_matrices(translation, rotation));
		matrix_t transform_terrain = multiply_matrices(get_view_projection(camera), multiply_matrices(translation_terrain, rotation));
		draw_mesh(mesh, transform, texture);
		draw_mesh(terrain, transform_terrain, texture_terrain);
		char fps[14];
		snprintf(fps, 14, "FPS: %f", 1.f / delta_time);
		wchar_t str[14];
		for (int i = 0; i < 14; i++) {
			str[i] = (wchar_t)fps[i];
		}
		plot_string(10, 10, str, (0 << 16) | (0 << 8) | 255, (0 << 16) | (255 << 8) | 0);
		render();
		res_update();
		timespec_get(&ts, TIME_UTC);
		end_time = (unsigned long long)ts.tv_sec * 1000000000 + (unsigned long long)ts.tv_nsec;
		delta_time = (end_time - start_time) * 0.000000001f;
	}
	free(camera);
	return 0;
}
