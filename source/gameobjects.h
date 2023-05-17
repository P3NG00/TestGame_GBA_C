#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <gba.h>

typedef struct Vector2
{
	// coordinates
	s16 x, y;
} Vector2;

typedef struct GameObject
{
	// stores whether the game object is active
	bool active;
	// game object position
	Vector2 position;
	// size
	u16 width, height;
	// color
	u16 color;
} GameObject;

typedef struct Player
{
	GameObject gameObject;
	// player facing direction
	Vector2 facing;
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
