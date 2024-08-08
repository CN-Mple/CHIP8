#include "../inc/chip_8_keyboard.h"

void keyboard_reset(struct chip_8_keyboard *keyboard)
{
	memset(keyboard->keybuffer, 0x00, sizeof(keyboard->keybuffer)); 
}

__attribute__((weak)) void keyboard_poll(struct chip_8_keyboard *keyboard)
{
	
}
