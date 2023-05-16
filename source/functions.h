#include <gba.h>

extern u16* VideoBuffer;

void SwapBuffers();
void WaitVBlank();
void FillScreen(u16 color);
void DrawSquare(u16 x, u16 y, u16 size, u16 color);
bool KeyHeld(u16 key);
