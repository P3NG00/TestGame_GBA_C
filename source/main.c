#include <gba.h>
#include "constants.h"
#include "functions.h"

#define COLOR_BLACK RGB5(0,   0,  0)
#define COLOR_WHITE RGB5(31, 31, 31)
#define COLOR_RED	RGB5(31,  0,  0)

// player info
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
Player player = { .x = DRAW_WIDTH / 2, .y = DRAW_HEIGHT / 2, .size = 4, .facing = 0, };

// function declarataions
void DrawGame();
void HandleInput();

// main code entry point
int main()
{
	// set screen mode 5, enable bkg2 for 16bit buffer
	REG_DISPCNT = (MODE_5 | BG2_ENABLE);
	// make backbuffer the first one we draw to while frontbuffer is displayed
	VideoBuffer = (u16*)MODE5_BB;
    // scale small mode 5 screen to full screen
    REG_BG2PA = 256 / 2;
    REG_BG2PD = 256 / 2;

	// game loop
    while (true)
    {
		// wait for vblank to complete
		WaitVBlank();
		// swap buffers
		SwapBuffers();
		// clear screen
		FillScreen(COLOR_BLACK);
		// draw game
		DrawGame();
		// handle input
		HandleInput();
    }
}

void DrawGame()
{
	// draw player
	u16 playerSizeHalf = player.size / 2;
	u16 drawX = player.x - playerSizeHalf;
	u16 drawY = player.y - playerSizeHalf;
	DrawSquare(drawX, drawY, player.size, COLOR_WHITE);
	// draw facing direction
	u16 drawFacingHeight = 0;
	u16 drawFacingWidth = 0;
	switch (player.facing)
	{
		// Up
		case 0 :
			drawFacingHeight = 3;
			drawFacingWidth = 2;
			drawX = player.x - 1;
			drawY = player.y - 2 - drawFacingHeight;
			break;

		// Right
		case 1 :
			drawFacingHeight = 2;
			drawFacingWidth = 3;
			drawX = player.x + 2;
			drawY = player.y - 1;
			break;

		// Down
		case 2 :
			drawFacingHeight = 3;
			drawFacingWidth = 2;
			drawX = player.x - 1;
			drawY = player.y + 2;
			break;

		// Left
		case 3 :
			drawFacingHeight = 2;
			drawFacingWidth = 3;
			drawX = player.x - 2 - drawFacingWidth;
			drawY = player.y - 1;
			break;
	}
	DrawRectangle(drawX, drawY, drawFacingWidth, drawFacingHeight, COLOR_RED);
}

void HandleInput()
{
	// handle movement input
	u16 playerSizeHalf = player.size / 2;
	if (KeyHeld(KEY_UP) && player.y > playerSizeHalf)
	{
		player.y--;
		player.facing = 0;
	}
	if (KeyHeld(KEY_DOWN) && player.y < DRAW_HEIGHT - playerSizeHalf)
	{
		player.y++;
		player.facing = 2;
	}
	if (KeyHeld(KEY_LEFT) && player.x > playerSizeHalf)
	{
		player.x--;
		player.facing = 3;
	}
	if (KeyHeld(KEY_RIGHT) && player.x < DRAW_WIDTH - playerSizeHalf)
	{
		player.x++;
		player.facing = 1;
	}
}
