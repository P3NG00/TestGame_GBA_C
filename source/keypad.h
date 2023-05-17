#include <gba.h>

extern u16 KeysPrevious;

void UpdateKeys();
bool KeyDownInKeyset(u16 key, u16 keys);
bool KeyHeld(u16 key);
bool KeyPressed(u16 key);
