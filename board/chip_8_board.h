#ifndef __CHIP_8_BOARD_H
#define __CHIP_8_BOARD_H

#include <stdio.h>

#include "../inc/chip_8_keyboard.h"
#include "../inc/chip_8_gpu.h"

#include <Windows.h>
#include <time.h>

#define CHIP_8_WIDTH	64
#define CHIP_8_HEIGHT	32

#define FRAME_RATE		60.0f
#define CPU_FREQUENCY	10000.0f

#define CPU_CYCLE		(1.0f / CPU_FREQUENCY)
#define DISPLAY_CYCLE	(1.0f / FRAME_RATE)

#define GET_TICK()		get_tick()
#define TICK_PER_SEC	(CLOCKS_PER_SEC / 1000)

typedef ULONGLONG tick_t;


#endif
