#ifndef __CHIP_8_EMULATOR_H
#define __CHIP_8_EMULATOR_H

#include <stdio.h>

#include "./chip_8_memory.h"
#include "./chip_8_cpu.h"
#include "./chip_8_gpu.h"
#include "./chip_8_keyboard.h"

#include "../board/chip_8_board.h"

struct chip_8_emulator {
    struct chip_8_memory memory;
    struct chip_8_cpu cpu;
    struct chip_8_gpu gpu;
    struct chip_8_keyboard keyboard;
    
    void (*init)(struct chip_8_emulator *emulator);
    void (*reset)(struct chip_8_emulator *emulator);
    void (*read)(struct chip_8_emulator *emulator, unsigned short const *code, unsigned short size);
    void (*run)(struct chip_8_emulator *emulator);
};

void emulator_init(struct chip_8_emulator *emulator);
void emulator_reset(struct chip_8_emulator *emulator);
void emulator_read(struct chip_8_emulator *emulator, unsigned short const *code, unsigned short size);
void emulator_run(struct chip_8_emulator *emulator);

#define default_struct_emulator()	{											\
										.memory = default_struct_memory(),		\
										.cpu = default_struct_cpu(),			\
										.gpu = default_struct_gpu(),			\
										.keyboard = default_struct_keyboard(),	\
										.init = emulator_init,					\
										.reset = emulator_reset,				\
										.read = emulator_read,					\
										.run = emulator_run,					\
									}

#endif
