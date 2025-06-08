#include "input.h"
#include "vector.h"
#include "quaternion.h"
#include <iso646.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <poll.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include <stddef.h>
#include <poll.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>

struct pollfd input_devices[MAX_OPEN_INPUT_DEVICES];
  
int num_devices;

bool pressed[256] = {false};
bool in_focus = true;

struct input_event ev[64];

struct termios orig_termios;

void reset_terminal_mode() {
	printf("\x1b[0m\x1b[2J\x1b[?25h\x1b[8;100;200t\x1b[?1004l");
	tcsetattr(STDOUT_FILENO, TCSANOW, &orig_termios);
}

bool is_character_device(const char *filename) {
  struct stat sb;
  return stat(filename, &sb) == 0 && (sb.st_mode & S_IFMT) == S_IFCHR;
}
unsigned long ev_get_capabilities(int fd) {
  unsigned long bits = 0;
  return ioctl(fd, EVIOCGBIT(0, sizeof(bits)), &bits) >= 0 ? bits : 0;
}

bool ev_has_key(int fd, unsigned int key)
{
  unsigned char bits[KEY_MAX / 8 + 1];
  memset(bits, 0, sizeof(bits));
  return ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(bits)), bits) >= 0 && (bits[key / 8] & (1 << (key % 8)));
}

void input_init() {
	tcgetattr(STDOUT_FILENO, &orig_termios);
  struct termios new_termios = orig_termios;
  new_termios.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDOUT_FILENO, TCSANOW, &new_termios);
	atexit(reset_terminal_mode);
  at_quick_exit(reset_terminal_mode);
  DIR * dir = opendir("/dev/input/");
  if (!dir) {
    fprintf( stderr, "Failure to open any devices.\n(Do you lack permissions for the /dev/input/ directory?)");
    exit(1);
  }

  struct dirent *dirent;
  while ((dirent = readdir(dir))) {
    if (strncmp(dirent->d_name, "event", 5) != 0){
      continue;
    }
    char filename[16] = "/dev/input/";
    strcat(filename, dirent->d_name);

    if (!is_character_device(filename)) {
      continue;
    }

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
      continue;
    }
    unsigned long device_capabilities = ev_get_capabilities(fd);
    if (device_capabilities == 0x120013 && ev_has_key(fd, KEY_ESC)) {
      input_devices[num_devices].fd = fd;
      input_devices[num_devices].events = POLLIN;
      num_devices++;
    }
    else {
      close(fd);
    }
  }
  closedir(dir);

  for (int n = 0; n < num_devices; n++) {
    ioctl(input_devices[n].fd, F_SETFL, ioctl(input_devices[n].fd, F_GETFL) | O_NONBLOCK);
  }
  printf("\x1b[?1004h");
}

void getch() {
  int curr_fd = -1;
  for (int n = 0; n < num_devices; n++) {
    input_devices[n].revents = 0;
  }
  if (poll(input_devices, num_devices, 0) > 0) {
    for (int n = 0; n < num_devices; n++) {
      if (input_devices[n].revents) {
        curr_fd = input_devices[n].fd;
      }
    }
  }
  if (curr_fd != -1) {
    int rd = read(curr_fd, ev, sizeof(struct input_event) * 64);
    for (long unsigned int i = 0; i < rd / sizeof(struct input_event); i++) {
      if (ev[i].type != EV_SYN && in_focus) {
        if (ev[i].value == 0) {
          pressed[ev[i].code] = false;
        }
        else if (ev[i].value == 1) {
          pressed[ev[i].code] = true;
        }
      }
    }
  }
}

void detect_focus() {
  struct pollfd fd;
  fd.fd = STDOUT_FILENO;
  fd.events = POLLIN;
  if (poll(&fd, 1, 0) > 0) {
    char c[3];
    read(STDOUT_FILENO, c, sizeof(c));
    if (strcmp(c, "\x1b[I") == 0) {
      in_focus = true;
    }
    else if (strcmp(c, "\x1b[O") == 0) {
      in_focus = false;
      memset(pressed, false, sizeof(pressed));
    }
  }
}
