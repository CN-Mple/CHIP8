#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./inc/chip_8_emulator.h"
#include "./inc/chip_8_rom_card.h"

static struct chip_8_emulator emulator = default_struct_emulator(); 

int main(int argc, char *argv[])
{
	system("cls");
	
	emulator.init(&emulator);
	emulator.reset(&emulator);
	emulator.read(&emulator, (unsigned short *)square_gif_code, sizeof(square_gif_code));
	emulator.run(&emulator);
	
	return 0;
}

