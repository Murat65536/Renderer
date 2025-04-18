CFLAGS = -lm -Wall -Wpedantic -Wextra -O3

all: renderer

renderer: main.o bitmap.o vector.o vertex.o gradients.o edge.o matrix.o display.o list.o indexedModel.o
	gcc $(CFLAGS) main.o bitmap.o vector.o vertex.o gradients.o edge.o matrix.o display.o list.o indexedModel.o -o renderer
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

indexedModel.o: indexedModel.c
	gcc $(CFLAGS) -c indexedModel.c

clean:
	rm main.o bitmap.o vector.o vertex.o gradients.o edge.o matrix.o display.o list.o indexedModel.o renderer
