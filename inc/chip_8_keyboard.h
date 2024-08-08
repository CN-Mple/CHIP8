#ifndef __CHIP_8_KEYBOARD_H
#define __CHIP_8_KEYBOARD_H

#include <stdio.h>
#include <string.h>

#include "../board/chip_8_board.h"

struct chip_8_keyboard {
	unsigned short keybuffer[16];
	
	void (*reset)(struct chip_8_keyboard *keyboard); 
	void (*poll)(struct chip_8_keyboard *keyboard);
};

void keyboard_reset(struct chip_8_keyboard *keyboard);
void keyboard_poll(struct chip_8_keyboard *keyboard);

#define default_struct_keyboard()	{					\
										{0},			\
										keyboard_reset,	\
										keyboard_poll,	\
									}

#endif
