#ifndef __CHIP_8_CPU_H
#define __CHIP_8_CPU_H

#include <stdio.h>
#include <stdlib.h>

#include "./chip_8_memory.h"
#include "./chip_8_gpu.h"
#include "./chip_8_keyboard.h"


struct chip_8_register {
    unsigned char V[16];
};

struct chip_8_op_instruction{
	unsigned short opcode: 4;
	unsigned short reserved:12;
};

struct chip_8_op_cmd{
	unsigned short opcode: 4;
	unsigned short zero:4;
	unsigned short cmd:8;
};

struct chip_8_op_immediate{
	unsigned short opcode: 4;
	unsigned short addr:12;
};

struct chip_8_op_x{
	unsigned short opcode:4;
	unsigned short x:4;
	unsigned short immediate:8;
};

struct chip_8_op_xy{
	unsigned short opcode:4;
	unsigned short x:4;
	unsigned short y:4;
	unsigned short immediate:4;
};
	
struct chip_8_ir {
	#if 0
    union{
    	unsigned short instruction;
    	struct chip_8_op_instruction op_instruction;
    	
    	struct chip_8_op_cmd op_cmd;
    	struct chip_8_op_immediate op_immediate;
    	struct chip_8_op_x op_x;
    	struct chip_8_op_xy op_xy;
	};
	#else
	unsigned short instruction;
	struct chip_8_op_instruction op_instruction;
	struct chip_8_op_cmd op_cmd;
	struct chip_8_op_immediate op_immediate;
	struct chip_8_op_x op_x;
	struct chip_8_op_xy op_xy;
	#endif
};

struct chip_8_cpu {
	struct chip_8_memory *memory;
    struct chip_8_keyboard *keyboard;
	struct chip_8_gpu *gpu;
	
    struct chip_8_register registers;
    struct chip_8_ir IR;
    
    unsigned short PC;
    unsigned char SP;
    unsigned short I;
    
    unsigned char delay_timer;
    unsigned char sound_timer;
    
    void (*init)(struct chip_8_cpu *cpu, struct chip_8_memory *memory, struct chip_8_gpu *gpu, struct chip_8_keyboard *keyboard);
    void (*reset)(struct chip_8_cpu *cpu);
    void (*cycle)(struct chip_8_cpu *cpu);
};

void cpu_init(struct chip_8_cpu *cpu, struct chip_8_memory *memory, struct chip_8_gpu *gpu, struct chip_8_keyboard *keyboard);
void cpu_reset(struct chip_8_cpu *cpu);
void cpu_cycle(struct chip_8_cpu *cpu);

#define default_struct_cpu()	{				\
									0,			\
									0,			\
									0,			\
									{0},		\
									{0},		\
									0,			\
									0,			\
									0,			\
									0,			\
									0,			\
									cpu_init,	\
									cpu_reset,	\
									cpu_cycle,	\
								}

#endif
