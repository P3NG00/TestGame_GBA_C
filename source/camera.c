#include <gba.h>
#include "gameobjects.h"
#include "util.h"

s16 camera_x = 0;
s16 camera_y = 0;

void UpdateCameraOffset(GameObject* gameObject)
{
    camera_x = gameObject->x - (DRAW_WIDTH / 2);
    camera_y = gameObject->y - (DRAW_HEIGHT / 2);
}
