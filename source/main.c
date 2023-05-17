#include <gba.h>
#include "functions.h"
#include "keypad.h"
#include "util.h"

// player
Player player = {
	.gameObject = {
		.x = DRAW_WIDTH / 2,
		.y = DRAW_HEIGHT / 2,
		.width = 4,
		.height = 4,
		.color = COLOR_WHITE
	},
	.facing = 0
};

// test wall
GameObject wall = {
	.x = DRAW_WIDTH / 4,
	.y = DRAW_HEIGHT / 2,
	.width = 4,
	.height = 24,
	.color = COLOR_WHITE
};

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
	// draw wall
	DrawGameObject(&wall);
	// draw player
	DrawGameObject(&player.gameObject);
	// draw facing direction
	u16 offset = (player.gameObject.width / 2) + 1;
	u16 drawX = player.gameObject.x;
	u16 drawY = player.gameObject.y;
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
	u16 playerSizeHalf = player.gameObject.width / 2;
	if (KeyHeld(KEY_UP) && player.gameObject.y > playerSizeHalf)
	{
		player.gameObject.y--;
		player.facing = 0;
	}
	if (KeyHeld(KEY_DOWN) && player.gameObject.y < DRAW_HEIGHT - playerSizeHalf)
	{
		player.gameObject.y++;
		player.facing = 2;
	}
	if (KeyHeld(KEY_LEFT) && player.gameObject.x > playerSizeHalf)
	{
		player.gameObject.x--;
		player.facing = 3;
	}
	if (KeyHeld(KEY_RIGHT) && player.gameObject.x < DRAW_WIDTH - playerSizeHalf)
	{
		player.gameObject.x++;
		player.facing = 1;
	}
	// handle collision
	// TODO
	// update keypad key tracking
	UpdateKeys();
}
