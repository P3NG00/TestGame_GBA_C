#include <gba.h>
#include "util.h"

// globals
u16 *VideoBuffer; // location we are currently drawing to, flips between front and back buffer

// function declarations
void SwapBuffers();
void WaitVBlank();
void FillScreen(u16 color);
void DrawRectangle(u16 x, u16 y, u16 width, u16 height, u16 color);
void DrawSquare(u16 x, u16 y, u16 size, u16 color);
void DrawRectangleCentered(u16 x, u16 y, u16 width, u16 height, u16 color);
void DrawSquareCentered(u16 x, u16 y, u16 size, u16 color);

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
    while (REG_VCOUNT < 160) {}
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
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            VideoBuffer[((y + i) * SCREEN_HEIGHT) + (x + j)] = color;
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
    u16 drawX = x - (width / 2);
    u16 drawY = y - (height / 2);
    DrawRectangle(drawX, drawY, width, height, color);
}

// draws a square centered at x, y with size and color
void DrawSquareCentered(u16 x, u16 y, u16 size, u16 color)
{
    DrawRectangleCentered(x, y, size, size, color);
}
