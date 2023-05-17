#include <gba.h>
#include "camera.h"
#include "functions.h"
#include "gameobjects.h"
#include "keypad.h"
#include "util.h"

#define PROJECTILE_LIMIT 16
#define PROJECTILE_SPEED  3

// game obejcts
Projectile projectiles[PROJECTILE_LIMIT];
GameObject walls[] = {
	{
		.active = true,
		.position = {
			.x = -15,
			.y = 0
		},
		.width = 4,
		.height = 24,
		.color = COLOR_WHITE
	}
};
Player player = {
	.gameObject = {
		.active = true,
		.position = {
			.x = 0,
			.y = 0
		},
		.width = 4,
		.height = 4,
		.color = COLOR_WHITE
	},
	.facing = 0
};

// function declarataions
void DrawGame();
void HandleCollision();
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

	// initialize game objects
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
		// handle collision
		HandleCollision();
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
			projectile->gameObject.position.x += projectile->dx;
			projectile->gameObject.position.y += projectile->dy;
		}
	}
}

void DrawGame()
{
	// clear screen
	FillScreen(COLOR_BLACK);
	// draw walls
	for (int i = 0; i < sizeof(walls) / sizeof(GameObject); i++)
		DrawGameObject(&walls[i]);
	// draw player
	DrawGameObject(&player.gameObject);
	// draw projectiles
	for (int i = 0; i < PROJECTILE_LIMIT; i++)
		DrawGameObject(&projectiles[i].gameObject);
	// draw facing direction
	u16 offset = (player.gameObject.width / 2) + 1;
	u16 drawX = player.gameObject.position.x;
	u16 drawY = player.gameObject.position.y;
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
		player.gameObject.position.y--;
		player.facing = 0;
	}
	if (KeyHeld(KEY_DOWN))
	{
		player.gameObject.position.y++;
		player.facing = 2;
	}
	if (KeyHeld(KEY_LEFT))
	{
		player.gameObject.position.x--;
		player.facing = 3;
	}
	if (KeyHeld(KEY_RIGHT))
	{
		player.gameObject.position.x++;
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
				s16 offset = player.gameObject.width / 2;
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
						.position = {
							.x = player.gameObject.position.x + (dx * offset),
							.y = player.gameObject.position.y + (dy * offset)
						},
						.width =  2,
						.height = 2,
						.color = COLOR_WHITE
					},
					.dx = dx * PROJECTILE_SPEED,
					.dy = dy * PROJECTILE_SPEED,
					.life = 60
				};
				break;
			}
		}
	}
	// update keypad key tracking
	UpdateKeys();
}

void HandleCollision()
{
	Projectile* projectile;
	// iterate through walls checking projectile and player collisions
	for (int i = 0; i < sizeof(walls) / sizeof(GameObject); i++)
	{
		// wall collision with projectile
		for (int j = 0; j < PROJECTILE_LIMIT; j++)
		{
			projectile = &projectiles[j];
			if (!projectile->gameObject.active)
				continue;
			if (ObjectsCollided(&walls[i], &projectile->gameObject))
				projectile->gameObject.active = false;
		}
		// wall collision with player
		if (ObjectsCollided(&walls[i], &player.gameObject))
		{
			// TODO
		}
	}
}
