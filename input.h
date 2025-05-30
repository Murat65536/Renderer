#ifndef INPUT_H_
#define INPUT_H_
#include <termios.h>
#include "camera.h"

extern struct termios orig_termios;

void reset_terminal_mode();

void input_init();

bool kbhit();

int getch(camera_t *camera, float delta_time);

#endif
