#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/kd.h>

struct termios orig_term_in;
struct termios orig_term_out;
int kdskb;

void reset_terminal_mode() {
  ioctl(STDOUT_FILENO, KDSKBMODE, kdskb);
  tcsetattr(STDOUT_FILENO, TCSANOW, &orig_term_in);
  tcsetattr(STDOUT_FILENO, TCSANOW, &orig_term_out);
}

int main() {
  ioctl(STDOUT_FILENO, KDGKBMODE, &kdskb);
  ioctl(STDOUT_FILENO, KDSKBMODE, K_RAW);
  struct termios new_term;
  tcgetattr(STDIN_FILENO, &orig_term_in);
  new_term = orig_term_in;
  cfmakeraw(&new_term);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
  tcgetattr(STDOUT_FILENO, &orig_term_out);
  new_term = orig_term_out;
	new_term.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDOUT_FILENO, TCSANOW, &new_term);
  atexit(reset_terminal_mode);
  at_quick_exit(reset_terminal_mode);
  char c;
  while (true) {
	  struct timeval tv = {0L, 0L};
	  fd_set fds;
	  FD_ZERO(&fds);
	  FD_SET(0, &fds);
	  if (select(1, &fds, NULL, NULL, &tv) > 0) {
      read(STDIN_FILENO, &c, sizeof(c));
      if (c == 'q') {
        return 0;
      }
      printf("%c\n", c);
    }
  }
  return 0;
}
