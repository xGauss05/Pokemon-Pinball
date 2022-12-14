#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 432
#define SCREEN_SIZE 2
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE false
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC false
#define TITLE "Pokemon Pinball"

// Scores
#define BUMPER_SCORE 500;
#define SLINGSHOT_SCORE 500;
#define PLUSLE_SCORE 2000;
#define MINUN_SCORE 2000;
#define ZIGZAGOON_SCORE 3000;
#define SEEDOT_SCORE 3000;
#define PELIPPER_SCORE 5000;
#define WAILMER_SCORE 10000;
#define PIKACHU_SCORE 10000;
#define SLINGSHOT_SCORE 500;
#define EVO_SCORE 2000;
#define GET_SCORE 3000;
#define UP_SCORE 1000;
#define HOLE_SCORE 1000;
