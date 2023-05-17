#include <gba.h>
#include "camera.h"
#include "gameobjects.h"
#include "util.h"

// globals
u16* VideoBuffer = (u16*)MODE5_BB;; // location we are currently drawing to, flips between front and back buffer, starting on backbuffer

// function declarations
void SwapBuffers();
void WaitVBlank();
void FillScreen(u16 color);
void DrawRectangle(u16 x, u16 y, u16 width, u16 height, u16 color);
void DrawSquare(u16 x, u16 y, u16 size, u16 color);
void DrawRectangleCentered(u16 x, u16 y, u16 width, u16 height, u16 color);
void DrawSquareCentered(u16 x, u16 y, u16 size, u16 color);
void DrawGameObject(GameObject* gameObject);

// this swaps the locations of the current display and current writing location
void SwapBuffers()
{
    // backbuffer is being displayed so swap
	if (REG_DISPCNT & BACKBUFFER)
	{
		REG_DISPCNT &= ~BACKBUFFER;	// make display show the frontbuffer (remove backbuffer using it's bitwise compliement)
		VideoBuffer = (u16*)MODE5_BB; // make backbuffer the one we draw to
	}
    // else frontbuffer is being displayed so swap
	else
	{
		REG_DISPCNT |= BACKBUFFER; // make display show the backbuffer (add backbuffer)
		VideoBuffer = (u16*)VRAM; // make frontbuffer the one we draw to
	}
}

// waits for vblank to complete
void WaitVBlank()
{
    while (REG_VCOUNT >= 160) {}
    while (REG_VCOUNT <  160) {}
}

// fills the screen pixels
void FillScreen(u16 color)
{
    int x, y;
    for (y = 0; y < DRAW_HEIGHT; y++)
    {
        for (x = 0; x < DRAW_WIDTH; x++)
        {
            VideoBuffer[(y * SCREEN_HEIGHT) + x] = color;
        }
    }
}

// draws a rectangle at x, y with width, height, and color
void DrawRectangle(u16 x, u16 y, u16 width, u16 height, u16 color)
{
    int i, j;
    s16 drawX, drawY;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            drawX = (x + j) - camera_x;
            drawY = (y + i) - camera_y;
            if (drawX < 0 || drawX >= DRAW_WIDTH || drawY < 0 || drawY >= DRAW_HEIGHT)
                continue;
            VideoBuffer[(drawY * SCREEN_HEIGHT) + drawX] = color;
        }
    }
}

// draws a square at x, y with size and color
void DrawSquare(u16 x, u16 y, u16 size, u16 color)
{
    DrawRectangle(x, y, size, size, color);
}

// draws a rectangle centered at x, y with width, height, and color
void DrawRectangleCentered(u16 x, u16 y, u16 width, u16 height, u16 color)
{
    u16 drawX = x - (width  / 2);
    u16 drawY = y - (height / 2);
    DrawRectangle(drawX, drawY, width, height, color);
}

// draws a square centered at x, y with size and color
void DrawSquareCentered(u16 x, u16 y, u16 size, u16 color)
{
    DrawRectangleCentered(x, y, size, size, color);
}

// draws a gameobject using it's x, y, width, height, and color
void DrawGameObject(GameObject* gameObject)
{
    if (!gameObject->active)
        return;
    DrawRectangleCentered(gameObject->position.x, gameObject->position.y, gameObject->width, gameObject->height, gameObject->color);
}

// checks if two objects are intersecting
bool ObjectsCollided(GameObject* gameObject0, GameObject* gameObject1)
{
    if (!gameObject0->active || !gameObject1->active)
        return false;
    return gameObject0->position.x - (gameObject0->width  / 2) < gameObject1->position.x + (gameObject1->width  / 2) &&
           gameObject0->position.x + (gameObject0->width  / 2) > gameObject1->position.x - (gameObject1->width  / 2) &&
           gameObject0->position.y - (gameObject0->height / 2) < gameObject1->position.y + (gameObject1->height / 2) &&
           gameObject0->position.y + (gameObject0->height / 2) > gameObject1->position.y - (gameObject1->height / 2);
}
