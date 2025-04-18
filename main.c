#include "main.h"
#include "vertex.h"
#include "matrix.h"
#include "display.h"
#include "bitmap.h"
#include <time.h>
#include <math.h>

int main() {
	initDisplay();
	vertex minYVert = {{-1.f, -1.f, 0.f, 1.f}, {0.f, 0.f, 0.f, 0.f}};
	vertex midYVert = {{0.f, 1.f, 0.f, 1.f}, {0.5f, 1.f, 0.f, 0.f}};
	vertex maxYVert = {{1.f, -1.f, 0.f, 1.f}, {1.f, 0.f, 0.f, 0.f}};

	bitmap file = newBitmapFile("simpbricks.png");

	float rotCounter = 0.f;
	unsigned long long startTime, deltaTime = 0;
	struct timespec ts;
	while (true) {
		timespec_get(&ts, TIME_UTC);
		startTime = (unsigned long long)ts.tv_sec * 1000000000 + (unsigned long long)ts.tv_nsec;
		if (kbhit()) {
			int c = getch();
			if (c == 'q') {	
				break;
			}
		}
		clear();
		rotCounter += deltaTime * 0.000000001f;
		matrix translation = initMatrixTranslation(0.f, 0.f, 3.f);
		matrix rotation = initMatrixRotation(rotCounter, rotCounter, rotCounter);
		matrix transform = multiplyMatricies(getProjectionMatrix(), multiplyMatricies(translation, rotation));
		fillTriangle(vertexTransform(maxYVert, transform), vertexTransform(midYVert, transform), vertexTransform(minYVert, transform)); 
		render();
		resUpdate();
		timespec_get(&ts, TIME_UTC);
		deltaTime = (unsigned long long)ts.tv_sec * 1000000000 + (unsigned long long)ts.tv_nsec - startTime;
	}
	return 0;
}
