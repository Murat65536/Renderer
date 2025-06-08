#include "main.h"
#include "vertex.h"
#include "matrix.h"
#include "display.h"
#include "bitmap.h"
#include "mesh.h"
#include "input.h"
#include "camera.h"
#include <time.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

int main(int argc, char **argv) {
	init_display();

	mesh_t *mesh = create_mesh("smoothMonkey0.obj", create_bitmap("bricks.png"), init_matrix_translation(0.f, 0.f, 1.f));
	mesh_t *terrain = create_mesh("terrain2.obj", create_bitmap("bricks2.png"), init_matrix_translation(0.f, -3.f, 0.f));
	camera_t *camera = malloc(sizeof(camera_t));
	camera->transform = (transform_t) {{0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}, {1.f, 1.f, 1.f, 1.f}};
	unsigned long long start_time, end_time = 0;
	float delta_time = 1.f;
	input_init();
	struct timespec ts;
	while (true) {
		timespec_get(&ts, TIME_UTC);
		start_time = (unsigned long long)ts.tv_sec * 1000000000 + (unsigned long long)ts.tv_nsec;
		camera->projection = projection;
    getch();
    detect_focus();
    if (camera_update(camera, delta_time) == -1) {
      break;
    }
    mesh->projection = multiply_matrices(mesh->projection, init_matrix_rotation(0.f, delta_time, 0.f));
		clear();
		draw_mesh(mesh, get_view_projection(camera));
		draw_mesh(terrain, get_view_projection(camera));
		//char fps[14];
		//snprintf(fps, 14, "FPS: %f", 1.f / delta_time);
		//wchar_t str[14];
		//for (int i = 0; i < 14; i++) {
		//	str[i] = (wchar_t)fps[i];
		//}
		//plot_string(10, 10, str, (0 << 16) | (0 << 8) | 255, (0 << 16) | (255 << 8) | 0);
		render();
		res_update();
		timespec_get(&ts, TIME_UTC);
		end_time = (unsigned long long)ts.tv_sec * 1000000000 + (unsigned long long)ts.tv_nsec;
		delta_time = (end_time - start_time) * 0.000000001f;
	}
  free_mesh(mesh);
  free_mesh(terrain);
	free(camera);
	return 0;
}
