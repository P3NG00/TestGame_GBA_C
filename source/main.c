#include <gba.h>
#include "constants.h"
#include "functions.h"

#define COLOR_BLACK RGB5(0, 0, 0)
#define COLOR_WHITE RGB5(31, 31, 31)

// player info
u16 playerX = DRAW_WIDTH / 2;
u16 playerY = DRAW_HEIGHT / 2;
const u16 playerSize = 4;
const u16 playerSizeHalf = playerSize / 2;

// function declarataions
void HandleInput();

// main code entry point
int main()
{
	// set screen mode 5, enable bkg2 for 16bit buffer
	REG_DISPCNT = (MODE_5 | BG2_ENABLE);
	// make backbuffer the first one we draw to while frontbuffer is displayed
	VideoBuffer = *MODE5_BB;
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
		// draw player
		DrawSquare(playerX - playerSizeHalf, playerY - playerSizeHalf, playerSize, COLOR_WHITE);
		// handle input
		HandleInput();
    }
}

void HandleInput()
{
	if (KeyHeld(KEY_UP) && playerY > playerSizeHalf)
		playerY--;
	if (KeyHeld(KEY_DOWN) && playerY < DRAW_HEIGHT - playerSizeHalf)
		playerY++;
	if (KeyHeld(KEY_LEFT) && playerX > playerSizeHalf)
		playerX--;
	if (KeyHeld(KEY_RIGHT) && playerX < DRAW_WIDTH - playerSizeHalf)
		playerX++;
}
