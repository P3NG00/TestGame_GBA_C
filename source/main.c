#include <gba.h>
#include "camera.h"
#include "functions.h"
#include "gameobjects.h"
#include "keypad.h"
#include "util.h"

#define PROJECTILE_LIMIT 16

// player
Player player = {
	.gameObject = {
		.active = true,
		.x = 0,
		.y = 0,
		.width = 4,
		.height = 4,
		.color = COLOR_WHITE
	},
	.facing = 0
};

// test wall
GameObject wall = {
	.active = true,
	.x = -15,
	.y = 0,
	.width = 4,
	.height = 24,
	.color = COLOR_WHITE
};

// projectiles
Projectile projectiles[PROJECTILE_LIMIT];

// function declarataions
void DrawGame();
void HandleInput();
void UpdateGameObjects();

// main code entry point
int main()
{
	// set screen mode 5, enable bkg2 for 16bit buffer
	REG_DISPCNT = (MODE_5 | BG2_ENABLE);
    // scale small mode 5 screen to full screen
    REG_BG2PA = 256 / 2;
    REG_BG2PD = 256 / 2;

	// initialize projectile array
	for (int i = 0; i < PROJECTILE_LIMIT; i++)
		projectiles[i] = (Projectile) { .gameObject = { .active = false } };

	// update camera offset
	UpdateCameraOffset(&player.gameObject);

	// game loop
    while (true)
    {
		// wait for vblank to complete
		WaitVBlank();
		// swap buffers
		SwapBuffers();
		// update camera offset
		UpdateCameraOffset(&player.gameObject);
		// handle input
		HandleInput();
		// update game objects
		UpdateGameObjects();
		// draw game
		DrawGame();
    }
}

void UpdateGameObjects()
{
	Projectile* projectile;
	// update projectiles
	for (int i = 0; i < PROJECTILE_LIMIT; i++)
	{
		projectile = &projectiles[i];
		if (projectile->gameObject.active)
		{
			// update projectile life
			projectile->life--;
			if (projectile->life == 0)
			{
				projectile->gameObject.active = false;
				continue;
			}
			// update projectile position
			projectile->gameObject.x += projectile->dx * projectile->speed;
			projectile->gameObject.y += projectile->dy * projectile->speed;
		}
	}
}

void DrawGame()
{
	// clear screen
	FillScreen(COLOR_BLACK);
	// draw wall
	DrawGameObject(&wall);
	// draw player
	DrawGameObject(&player.gameObject);
	// draw projectiles
	for (int i = 0; i < PROJECTILE_LIMIT; i++)
		DrawGameObject(&projectiles[i].gameObject);
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
	if (KeyHeld(KEY_UP))
	{
		player.gameObject.y--;
		player.facing = 0;
	}
	if (KeyHeld(KEY_DOWN))
	{
		player.gameObject.y++;
		player.facing = 2;
	}
	if (KeyHeld(KEY_LEFT))
	{
		player.gameObject.x--;
		player.facing = 3;
	}
	if (KeyHeld(KEY_RIGHT))
	{
		player.gameObject.x++;
		player.facing = 1;
	}
	// handle shooting input
	if (KeyPressed(KEY_A))
	{
		for (int i = 0; i < PROJECTILE_LIMIT; i++)
		{
			if (!projectiles[i].gameObject.active)
			{
				s16 dx = 0;
				s16 dy = 0;
				switch (player.facing)
				{
					case 0 : dy--; break; // Up
					case 1 : dx++; break; // Right
					case 2 : dy++; break; // Down
					case 3 : dx--; break; // Left
				}
				projectiles[i] = (Projectile) {
					.gameObject = {
						.active = true,
						.x = player.gameObject.x,
						.y = player.gameObject.y,
						.width =  2,
						.height = 2,
						.color = COLOR_WHITE
					},
					.dx = dx,
					.dy = dy,
					.life = 60,
					.speed = 3
				};
				break;
			}
		}
	}
	// handle collision
	// TODO
	// update keypad key tracking
	UpdateKeys();
}
