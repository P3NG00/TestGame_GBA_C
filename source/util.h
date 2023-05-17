#ifndef UTIL_H
#define UTIL_H

#include <gba_video.h>

#define COLOR_BLACK RGB5(0,   0,  0)
#define COLOR_WHITE RGB5(31, 31, 31)

#define DRAW_WIDTH SCREEN_WIDTH / 2
#define DRAW_HEIGHT SCREEN_HEIGHT / 2

typedef struct GameObject
{
	// coordinates
	u16 x, y;
	// size
	u16 width, height;
	// color
	u16 color;
} GameObject;

typedef struct Player
{
	GameObject gameObject;
	// player facing direction
	// 0 = Up
	// 1 = Right
	// 2 = Down
	// 3 = Left
	u8 facing;
} Player;

#endif
