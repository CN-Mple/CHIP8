#ifndef __CHIP_8_MEMORY_H
#define __CHIP_8_MEMORY_H

#include <stdio.h>
#include <string.h>

struct chip_8_mem_reserved {
	union {
		unsigned char system_data[512];
		struct {
			unsigned char reserved0[80];
        	unsigned char font_bitmap[80];
			unsigned char reserved1[342];
		};
	};
};

struct chip_8_mem_code {
	unsigned char data[3232];
};

struct chip_8_mem_stack {
	unsigned char data[96];
};

struct chip_8_mem_vram {
	unsigned char data[256];
};

struct chip_8_mem_ram {
	union {
        unsigned char data[4096];
        struct {
            struct chip_8_mem_reserved system_reserved;
            struct chip_8_mem_code code;
            struct chip_8_mem_stack stack;
            struct chip_8_mem_vram vram;
        };
    };
};

#define CHIP_8_BASE				(unsigned short)0x000
#define CHIP_8_CODE_OFFSER		(unsigned short)0x1FF
#define CHIP_8_STACK_OFFSER		(unsigned short)0xEA0
#define CHIP_8_VRAM_OFFSER		(unsigned short)0xF00

#define CHIP_8_RAM_WRITE(x, addr, byte)		memory->write(x->memory, (CHIP_8_BASE + addr), byte)
#define CHIP_8_RAM_READ(x, addr)			memory->read(x->memory, (CHIP_8_BASE + addr))

#define CHIP_8_STACK_WRITE(x, byte)			memory->write(x->memory, (CHIP_8_BASE + CHIP_8_STACK_OFFSER + cpu->SP), byte)
#define CHIP_8_STACK_READ(x)				memory->read(x->memory, (CHIP_8_BASE + CHIP_8_STACK_OFFSER + cpu->SP))

#define CHIP_8_VRAM_WRITE(x, addr, byte)	memory->write(x->memory, (CHIP_8_BASE + CHIP_8_VRAM_OFFSER + addr), byte)
#define CHIP_8_VRAM_READ(x, addr)			memory->read(x->memory, (CHIP_8_BASE + CHIP_8_VRAM_OFFSER + addr))

struct chip_8_memory {
	void (*reset)(struct chip_8_memory *memory);
	void (*write)(struct chip_8_memory *memory, unsigned short addr, unsigned char byte);
	unsigned char (*read)(struct chip_8_memory *memory, unsigned short addr);
};

void memory_reset(struct chip_8_memory *memory);
void memory_write(struct chip_8_memory *memory, unsigned short addr, unsigned char byte);
unsigned char memory_read(struct chip_8_memory *memory, unsigned short addr);

static const unsigned char font_data[80];

#define default_struct_memory()	{					\
									memory_reset,	\
									memory_write,	\
									memory_read,	\
								} 

#endif
