#include <gba.h>
#include "util.h"

s16 camera_x = 0;
s16 camera_y = 0;

void UpdateCameraOffset(GameObject* gameObject)
{
    camera_x = gameObject->x - (s16)(DRAW_WIDTH / 2);
    camera_y = gameObject->y - (s16)(DRAW_HEIGHT / 2);
}
