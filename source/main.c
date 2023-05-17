#include <gba.h>
#include "camera.h"
#include "functions.h"
#include "gameobjects.h"
#include "keypad.h"
#include "util.h"

#define PROJECTILE_LIMIT 16
#define PROJECTILE_SPEED  3

#define PLAYER_SIZE 4

// game obejcts
Projectile projectiles[PROJECTILE_LIMIT];
GameObject walls[] = {
	{
		.active = true,
		.position = {
			.x = -12,
			.y =   0
		},
		.width  =  4,
		.height = 24,
		.color = COLOR_WHITE
	},
	{
		.active = true,
		.position = {
			.x =   0,
			.y = -24
		},
		.width  = 30,
		.height =  4,
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
		.width  = PLAYER_SIZE,
		.height = PLAYER_SIZE,
		.color = COLOR_WHITE
	},
	.facing = {
		.x = 0,
		.y = 0
	}
};
GameObject playerTestObjectX = {
	.active = true,
	.position = {
		.x = 0,
		.y = 0
	},
	.width  = PLAYER_SIZE,
	.height = PLAYER_SIZE,
};
GameObject playerTestObjectY = {
	.active = true,
	.position = {
		.x = 0,
		.y = 0
	},
	.width  = PLAYER_SIZE,
	.height = PLAYER_SIZE
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
		if (!projectile->gameObject.active)
			continue;
		// update projectile life
		if (--projectile->life == 0)
		{
			projectile->gameObject.active = false;
			continue;
		}
		// update projectile position
		projectile->gameObject.position.x += projectile->dx;
		projectile->gameObject.position.y += projectile->dy;
	}
}

void DrawGame()
{
	// clear screen
	FillScreen(COLOR_BLACK);
	// draw walls
	for (int i = 0; i < sizeof(walls) / sizeof(GameObject); i++)
		DrawGameObject(&walls[i]);
	// draw projectiles
	for (int i = 0; i < PROJECTILE_LIMIT; i++)
		DrawGameObject(&projectiles[i].gameObject);
	// draw player
	DrawGameObject(&player.gameObject);
	// draw facing direction
	u16 playerHalfWidth = PLAYER_SIZE / 2;
	u16 offset = playerHalfWidth;
	if (player.facing.x == 0 || player.facing.y == 0)
		offset++;
	u16 drawX = player.gameObject.position.x + (player.facing.x * offset);
	u16 drawY = player.gameObject.position.y + (player.facing.y * offset);
	DrawSquareCentered(drawX, drawY, playerHalfWidth, COLOR_WHITE);
}

void HandleInput()
{
	playerTestObjectX.position = player.gameObject.position;
	playerTestObjectY.position = player.gameObject.position;
	// TODO store player's last facing direction if it's going to be set to 0, 0
	player.facing.x = 0;
	player.facing.y = 0;
	// handle movement input
	if (KeyHeld(KEY_UP))
	{
		playerTestObjectY.position.y--;
		player.facing.y--;
	}
	if (KeyHeld(KEY_DOWN))
	{
		playerTestObjectY.position.y++;
		player.facing.y++;
	}
	if (KeyHeld(KEY_LEFT))
	{
		playerTestObjectX.position.x--;
		player.facing.x--;
	}
	if (KeyHeld(KEY_RIGHT))
	{
		playerTestObjectX.position.x++;
		player.facing.x++;
	}
	// handle shooting input
	if (KeyPressed(KEY_A))
	{
		for (int i = 0; i < PROJECTILE_LIMIT; i++)
		{
			if (!projectiles[i].gameObject.active)
			{
				s16 dx = player.facing.x;
				s16 dy = player.facing.y;
				s16 offset = PLAYER_SIZE / 2;
				projectiles[i] = (Projectile) {
					.gameObject = {
						.active = true,
						.position = {
							.x = player.gameObject.position.x + (dx * offset),
							.y = player.gameObject.position.y + (dy * offset)
						},
						.width  = 2,
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
	bool playerCollidedX = false;
	bool playerCollidedY = false;
	Projectile* projectile;
	GameObject* wall;
	// iterate through walls checking projectile and player collisions
	for (int i = 0; i < sizeof(walls) / sizeof(GameObject); i++)
	{
		wall = &walls[i];
		// wall collision with projectile
		for (int j = 0; j < PROJECTILE_LIMIT; j++)
		{
			projectile = &projectiles[j];
			if (!projectile->gameObject.active)
				continue;
			if (ObjectsCollided(wall, &projectile->gameObject))
				projectile->gameObject.active = false;
		}
		// wall collision with player
		if (!playerCollidedX && ObjectsCollided(wall, &playerTestObjectX))
			playerCollidedX = true;
		if (!playerCollidedY && ObjectsCollided(wall, &playerTestObjectY))
			playerCollidedY = true;
	}
	if (!playerCollidedX)
		player.gameObject.position.x = playerTestObjectX.position.x;
	if (!playerCollidedY)
		player.gameObject.position.y = playerTestObjectY.position.y;
}
