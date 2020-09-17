#pragma once

#include <allegro5/allegro.h>

#define GRID_SIZE 1
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

typedef unsigned short Dim;

enum class HFrameAlign {
	LEFT, CENTER, RIGHT
};

enum class VFrameAlign {
	BOTTOM, CENTER, TOP
};

typedef ALLEGRO_BITMAP* Bitmap_t;

struct Rect {
	Dim x;
	Dim y;
	Dim w;
	Dim h;
};

struct Point {
	Dim x;
	Dim y;
};

struct Layer {
	Dim width, height, border_west, border_east, border_north, border_south;
};