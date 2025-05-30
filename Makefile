CFLAGS = -lm -Wall -Wpedantic -Wextra -g3
# -g3 flag for debug
# -O3 for performance

all: renderer

renderer: main.o bitmap.o vector.o vertex.o gradients.o edge.o matrix.o display.o list.o indexed_model.o obj_model.o hashmap.o lodepng.o mesh.o math.o quaternion.o transform.o camera.o input.o
	gcc $(CFLAGS) main.o bitmap.o vector.o vertex.o gradients.o edge.o matrix.o display.o list.o indexed_model.o obj_model.o hashmap.o lodepng.o mesh.o math.o quaternion.o transform.o camera.o input.o -o renderer
	chmod +x renderer

main.o: main.c
	gcc $(CFLAGS) -c main.c

bitmap.o: bitmap.c
	gcc $(CFLAGS) -c bitmap.c

vector.o: vector.c
	gcc $(CFLAGS) -c vector.c

vertex.o: vertex.c
	gcc $(CFLAGS) -c vertex.c

gradients.o: gradients.c
	gcc $(CFLAGS) -c gradients.c

edge.o: edge.c
	gcc $(CFLAGS) -c edge.c

matrix.o: matrix.c
	gcc $(CFLAGS) -c matrix.c

display.o: display.c
	gcc $(CFLAGS) -c display.c

list.o: list.c
	gcc $(CFLAGS) -c list.c

indexed_model.o: indexed_model.c
	gcc $(CFLAGS) -c indexed_model.c

obj_model.o: obj_model.c
	gcc $(CFLAGS) -c obj_model.c

hashmap.o: hashmap.c
	gcc $(CFLAGS) -c hashmap.c

lodepng.o: lodepng.c
	gcc $(CFLAGS) -c lodepng.c

mesh.o: mesh.c
	gcc $(CFLAGS) -c mesh.c

math.o: math.c
	gcc $(CFLAGS) -c math.c

quaternion.o: quaternion.c
	gcc $(CFLAGS) -c quaternion.c

transform.o: transform.c
	gcc $(CFLAGS) -c transform.c

camera.o: camera.c
	gcc $(CFLAGS) -c camera.c

input.o: input.c
	gcc $(CFLAGS) -c input.c

clean:
	rm main.o bitmap.o vector.o vertex.o gradients.o edge.o matrix.o display.o list.o indexed_model.o obj_model.o hashmap.o lodepng.o mesh.o math.o quaternion.o transform.o camera.o input.o renderer
