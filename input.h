#ifndef INPUT_H_
#define INPUT_H_
#include <termios.h>
#include "camera.h"
#include <stdbool.h>
#include <stddef.h>
#include <poll.h>

#define MAX_OPEN_INPUT_DEVICES 10

extern struct termios orig_termios;
extern bool pressed[256];

void reset_terminal_mode();

void input_init();

void getch();

void detect_focus();

#endif
