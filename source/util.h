#include <gba_video.h>

#define COLOR_BLACK RGB5(0,   0,  0)
#define COLOR_WHITE RGB5(31, 31, 31)
#define COLOR_RED	RGB5(31,  0,  0)

#define DRAW_WIDTH SCREEN_WIDTH / 2
#define DRAW_HEIGHT SCREEN_HEIGHT / 2

typedef struct Player
{
	// player coordinates
	u16 x, y;
	// player size
	u16 size;
	// player facing direction
	// 0 = Up
	// 1 = Right
	// 2 = Down
	// 3 = Left
	u8 facing;
} Player;
