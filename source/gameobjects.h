#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <gba.h>

typedef struct GameObject
{
	// stores whether the game object is active
	bool active;
	// coordinates
	s16 x, y;
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

typedef struct Projectile
{
	GameObject gameObject;
	// projectile direction
	s16 dx, dy;
	// projectile life
	u8 life;
} Projectile;

#endif
