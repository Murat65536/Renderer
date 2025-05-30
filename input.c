#include "input.h"
#include "vector.h"
#include "quaternion.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

#define MOVEMENT_SPEED 5.f
#define SENSITIVITY 5.f

struct termios orig_termios;

void reset_terminal_mode() {
	printf("\x1b[0m\x1b[2J\x1b[?25h\x1b[8;100;200t");
	tcsetattr(STDOUT_FILENO, TCSANOW, &orig_termios);
}

void input_init() {
	tcgetattr(STDOUT_FILENO, &orig_termios);
	struct termios new_termios = orig_termios;
	new_termios.c_lflag &= ~(ICANON | ECHO | ISIG);
	new_termios.c_iflag = 0;
	new_termios.c_cc[VMIN] = 0xff;
	new_termios.c_cc[VTIME] = 1;
	tcsetattr(STDOUT_FILENO, TCSANOW, &new_termios);
	atexit(reset_terminal_mode);
}

bool kbhit() {
	struct timeval tv = {0L, 0L};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv) > 0;
}

int getch(camera_t *camera, float delta_time) {
	unsigned char c;
	const vector_t Y_AXIS = (vector_t) {0.f, 1.f, 0.f, 1.f};
	const vector_t X_AXIS = (vector_t) {1.f, 0.f, 0.f, 1.f};
	read(0, &c, sizeof(c));
	switch (c) {
		case 'q':
			return -1;
		case 'w':
			camera_move(camera, quaternion_forward(camera->transform.rot), MOVEMENT_SPEED * delta_time);
			break;
		case 'a':
			camera_move(camera, quaternion_left(camera->transform.rot), MOVEMENT_SPEED * delta_time);
			break;
		case 's':
			camera_move(camera, quaternion_back(camera->transform.rot), MOVEMENT_SPEED * delta_time);
			break;
		case 'd':
			camera_move(camera, quaternion_right(camera->transform.rot), MOVEMENT_SPEED * delta_time);
			break;
		case ' ':
			camera_move(camera, quaternion_up(camera->transform.rot), MOVEMENT_SPEED * delta_time);
			break;
		case 'z':
			camera_move(camera, quaternion_down(camera->transform.rot), MOVEMENT_SPEED * delta_time);
			break;
		case 'i':
			camera_rotate(camera, X_AXIS, -SENSITIVITY * delta_time);
			break;
		case 'j':
			camera_rotate(camera, Y_AXIS, -SENSITIVITY * delta_time);
			break;
		case 'k':
			camera_rotate(camera, X_AXIS, SENSITIVITY * delta_time);
			break;
		case 'l':
			camera_rotate(camera, Y_AXIS, SENSITIVITY * delta_time);
			break;
	}
	return 0;
}
