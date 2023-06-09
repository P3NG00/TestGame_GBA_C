#include <gba.h>
#include "gameobjects.h"
#include "util.h"

extern u16* VideoBuffer;

void SwapBuffers();
void WaitVBlank();
void FillScreen(u16 color);
void DrawRectangle(u16 x, u16 y, u16 width, u16 height, u16 color);
void DrawSquare(u16 x, u16 y, u16 size, u16 color);
void DrawRectangleCentered(u16 x, u16 y, u16 width, u16 height, u16 color);
void DrawSquareCentered(u16 x, u16 y, u16 size, u16 color);
void DrawGameObject(GameObject* gameObject);
bool ObjectsCollided(GameObject* gameObject0, GameObject* gameObject1);
