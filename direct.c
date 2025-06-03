#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>


int main(int argc, char **argv) {
  int fd, rd;
	struct input_event ev[64];
	char name[256] = "Unknown";

	if (argc < 2) {
		printf("Usage: evtest /dev/input/eventX\n");
		printf("Where X = input device number\n");
		return 1;
	}

	if ((fd = open(argv[argc - 1], O_RDONLY)) < 0) {
		perror("evtest");
		return 1;
	}

	ioctl(fd, EVIOCGNAME(sizeof(name)), name);
	printf("Input device name: \"%s\"\n", name);

	while (1) {
		rd = read(fd, ev, sizeof(struct input_event) * 64);
		if (rd < (int) sizeof(struct input_event)) {
			perror("\nevtest: error reading");
			return 1;
		}

		for (int i = 0; i < rd / sizeof(struct input_event); i++)

			if (ev[i].type != EV_SYN) {
				printf("Event: time %ld.%06ld, type %d (%d), code %d (%d), value %d\n",
					ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type,
					ev[i].type,
					ev[i].code,
					ev[i].type,
					ev[i].value);
			}

	}
}
