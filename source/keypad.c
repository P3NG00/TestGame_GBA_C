#include <gba.h>

// globals
u16 KeysPrevious = 0;

// function declarations
void UpdateKeys();
bool KeyDownInKeyset(u16 key, u16 keys);
bool KeyHeld(u16 key);
bool KeyPressed(u16 key);

// updates the previous keys
void UpdateKeys()
{
    KeysPrevious = REG_KEYINPUT;
}

// returns if the key is held in this keyset
bool KeyDownInKeyset(u16 key, u16 keys)
{
    return ~keys & key;
}

// returns if the key is held
bool KeyHeld(u16 key)
{
    return KeyDownInKeyset(key, REG_KEYINPUT);
}

// returns if the key was pressed this frame
bool KeyPressed(u16 key)
{
    return !KeyDownInKeyset(key, KeysPrevious) && KeyHeld(key);
}
