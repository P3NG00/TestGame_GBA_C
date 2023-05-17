#include <gba.h>
#include "util.h"
#include "functions.h"
#include "keypad.h"

// player
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
		// handle input
		HandleInput();
		// swap buffers
		SwapBuffers();
		// clear screen
		FillScreen(COLOR_BLACK);
		// draw game
		DrawGame();
    }
}

void DrawGame()
{
	// draw player
	DrawSquareCentered(player.x, player.y, player.size, COLOR_WHITE);
	// draw facing direction
	u16 offset = (player.size / 2) + 1;
	u16 drawX = player.x;
	u16 drawY = player.y;
	switch (player.facing)
	{
		case 0 : drawY -= offset; break; // Up
		case 1 : drawX += offset; break; // Right
		case 2 : drawY += offset; break; // Down
		case 3 : drawX -= offset; break; // Left
	}
	DrawSquareCentered(drawX, drawY, 2, COLOR_WHITE);
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
	// update keypad key tracking
	UpdateKeys();
}
