#include "../inc/chip_8_cpu.h"

static void push(struct chip_8_cpu *cpu)
{
	unsigned char high,low;
	high = (unsigned char)(cpu->PC >> 8); 
	low = (unsigned char)(cpu->PC & 0xFF);
	
	cpu->SP--;
	cpu->CHIP_8_STACK_WRITE(cpu, high);
	cpu->SP--;
	cpu->CHIP_8_STACK_WRITE(cpu, low);
}

static void pop(struct chip_8_cpu *cpu)
{
	unsigned char high,low;
	high = (unsigned char)(cpu->PC >> 8); 
	low = (unsigned char)(cpu->PC & 0xFF);
	cpu->PC = 0x0000;
	
	low  = cpu->CHIP_8_STACK_READ(cpu);
	cpu->SP++;
	high = cpu->CHIP_8_STACK_READ(cpu);
	cpu->SP++;
	
	cpu->PC =  high << 8 | low;
}

static void fetch(struct chip_8_cpu *cpu)
{
	cpu->IR.instruction = cpu->CHIP_8_RAM_READ(cpu, cpu->PC) << 8 | cpu->CHIP_8_RAM_READ(cpu, cpu->PC + 1);
	cpu->PC += 2;
}

static void decode(struct chip_8_cpu *cpu)
{
	cpu->IR.op_instruction.opcode = (cpu->IR.instruction & 0xF000) >> 12;
	
	cpu->IR.op_cmd.opcode = (cpu->IR.instruction & 0xF000) >> 12;
	cpu->IR.op_cmd.zero = (cpu->IR.instruction & 0x0F00) >> 8;
	cpu->IR.op_cmd.cmd = (cpu->IR.instruction & 0x00FF);
	
	cpu->IR.op_immediate.opcode = (cpu->IR.instruction & 0xF000) >> 12;
	cpu->IR.op_immediate.addr = (cpu->IR.instruction & 0x0FFF);
	
	cpu->IR.op_x.opcode = (cpu->IR.instruction & 0xF000) >> 12;
	cpu->IR.op_x.x = (cpu->IR.instruction & 0x0F00) >> 8;
	cpu->IR.op_x.immediate = (cpu->IR.instruction & 0x00FF);
	
	cpu->IR.op_xy.opcode = (cpu->IR.instruction & 0xF000) >> 12;
	cpu->IR.op_xy.x = (cpu->IR.instruction & 0x0F00) >> 8;
	cpu->IR.op_xy.y = (cpu->IR.instruction & 0x00F0) >> 4;
	cpu->IR.op_xy.immediate = (cpu->IR.instruction & 0x000F);
	
	switch(cpu->IR.op_instruction.opcode) {
		case 0x0:
			if(cpu->IR.op_cmd.zero != 0) {
				
			}else {
				switch(cpu->IR.op_cmd.cmd) {
					case 0x00:
						cpu->PC -= 2;
					break;
					case 0xE0:/* clean screen */
						cpu->gpu->fill(cpu->gpu, 0);
						cpu->gpu->draw_frame_flag = 1;
					break;
					case 0xEE:/* ret */
						pop(cpu);
					break;
				}
			}
		break;
		/* 1NNN */
		case 0x1:/* jmp to nnn */
			cpu->PC = cpu->IR.op_immediate.addr;
		break;
		/* 2NNN */
		case 0x2:/* call addr */
			push(cpu);
			cpu->PC = cpu->IR.op_immediate.addr;
		break;
		/* 3XNN */
		case 0x3:/* se vx, byte */
			if(cpu->IR.op_x.immediate == cpu->registers.V[cpu->IR.op_x.x]) {
				cpu->PC += 2;
			}
		break;
		/* 4XNN */
		case 0x4:/* sne vx, byte */
			if(cpu->IR.op_x.immediate != cpu->registers.V[cpu->IR.op_x.x]) {
				cpu->PC += 2;
			}
		break;
		/* 5XY0 */
		case 0x5:/* se vx, vy */
			if(cpu->registers.V[cpu->IR.op_xy.x] == cpu->registers.V[cpu->IR.op_xy.y]) {
				cpu->PC += 2;
			}
		break;
		/* 6XNN */
		case 0x6:/* ld vx, byte */
			cpu->registers.V[cpu->IR.op_x.x] = cpu->IR.op_x.immediate;
		break;
		/* 7XNN */
		case 0x7:/* add vx, byte */
			cpu->registers.V[cpu->IR.op_x.x] += cpu->IR.op_x.immediate;
		break;
		/* 8XYN */
		case 0x8:
			switch(cpu->IR.op_xy.immediate) {
				case 0x0:/* ld vx, vy */
					cpu->registers.V[cpu->IR.op_xy.x] = cpu->registers.V[cpu->IR.op_xy.y];
				break;
				case 0x1:/* or vx, vy */
					cpu->registers.V[cpu->IR.op_xy.x] |= cpu->registers.V[cpu->IR.op_xy.y];
				break;
				case 0x2:/* and vx, vy */
					cpu->registers.V[cpu->IR.op_xy.x] &= cpu->registers.V[cpu->IR.op_xy.y];
				break;
				case 0x3:/* xor vx, vy */
					cpu->registers.V[cpu->IR.op_xy.x] ^= cpu->registers.V[cpu->IR.op_xy.y];
				break;
				case 0x4:/* add vx, vy */
					if((cpu->registers.V[cpu->IR.op_xy.x] & cpu->registers.V[cpu->IR.op_xy.y] & 0x80) == 0x80) {
						cpu->registers.V[0xF] = 0x01;
					}else {
						cpu->registers.V[0xF] = 0x00;
					}
					cpu->registers.V[cpu->IR.op_xy.x] += cpu->registers.V[cpu->IR.op_xy.y];
				break;
				case 0x5:/* sub vx, vy */
					if(cpu->registers.V[cpu->IR.op_xy.x] < cpu->registers.V[cpu->IR.op_xy.y]) {
						cpu->registers.V[0xF] = 0x00;
					}else {
						cpu->registers.V[0xF] = 0x01;
					}
					cpu->registers.V[cpu->IR.op_xy.x] -= cpu->registers.V[cpu->IR.op_xy.y];
				break;
				case 0x6:/* shr vx, 1 */
					cpu->registers.V[0xF] = cpu->registers.V[cpu->IR.op_xy.x] & 0x01;
					cpu->registers.V[cpu->IR.op_xy.x] >>= 1;
				break;
				case 0x7:/* subn vx, vy */
					if(cpu->registers.V[cpu->IR.op_xy.y] < cpu->registers.V[cpu->IR.op_xy.x]) {
						cpu->registers.V[0xF] = 0x00;
					}else {
						cpu->registers.V[0xF] = 0x01;
					}
					cpu->registers.V[cpu->IR.op_xy.x] = cpu->registers.V[cpu->IR.op_xy.y] - cpu->registers.V[cpu->IR.op_xy.x];
				break;
				case 0xE:/* shl vx, 1 */
					cpu->registers.V[0xF] = (cpu->registers.V[cpu->IR.op_xy.x] >> 7) & 0x01;
					cpu->registers.V[cpu->IR.op_xy.x] <<= 1;
				break;
			}
		break;
		/* 9XY0 */
		case 0x9:/* se vx, vy */
			if(cpu->registers.V[cpu->IR.op_xy.x] != cpu->registers.V[cpu->IR.op_xy.y]) {
				cpu->PC += 2;
			}
		break;
		/* ANNN */
		case 0xA:/* ld i, nnn */
			cpu->I = cpu->IR.op_immediate.addr;
		break;
		/* BNNN */
		case 0xB:/* jmp v0, addr */
			cpu->PC = cpu->registers.V[0x0] + cpu->IR.op_immediate.addr;
		break;
		/* CXNN */
		case 0xC:/* rnd vx, byte */
			cpu->registers.V[cpu->IR.op_x.x] = rand() & cpu->IR.op_x.immediate;
		break;
		/* DXYN */
		case 0xD:/* draw vx, vy, n */
			;
			unsigned char x = cpu->registers.V[cpu->IR.op_xy.x];
			unsigned char y = cpu->registers.V[cpu->IR.op_xy.y];
			unsigned char n = cpu->IR.op_xy.immediate;
			
			unsigned char bitmap_byte;
			unsigned char bitmap_bit;
			for(unsigned char yy = 0; yy < n; ++yy) {
				bitmap_byte = cpu->CHIP_8_RAM_READ(cpu, cpu->I + yy);
				for(unsigned char xx = 0; xx < 8; ++xx) {
					x = cpu->registers.V[cpu->IR.op_xy.x] + xx;
					y = cpu->registers.V[cpu->IR.op_xy.y] + yy;
					
					unsigned char vram_byte = cpu->gpu->CHIP_8_VRAM_READ(cpu, (y / 8) * cpu->gpu->width + x);
					if(x < cpu->gpu->width && y < cpu->gpu->height) {
						bitmap_bit = (bitmap_byte >> (7 - xx)) & 0x01;
						if(vram_byte & (bitmap_bit << (y % 8))) {
							cpu->registers.V[0xF] = 0x01;
						}
						vram_byte ^= (bitmap_bit << (y % 8));
						cpu->gpu->CHIP_8_VRAM_WRITE(cpu, (y / 8) * cpu->gpu->width + x, vram_byte);
					}
				}
			}
			cpu->gpu->draw_frame_flag = 1;
			
		break;
		/* EXNN */
		case 0xE:
			switch(cpu->IR.op_x.immediate){
				case 0x9E:/* skp vx */
					if(cpu->keyboard->keybuffer[cpu->registers.V[cpu->IR.op_x.x]] == 0x01) {
						cpu->PC += 2;
					}
				break;
				case 0xA1:/* sknp vx */
					if(cpu->keyboard->keybuffer[cpu->registers.V[cpu->IR.op_x.x]] == 0x00) {
						cpu->PC += 2;
					}
				break;
			}
		break;
		/* FXNN */
		case 0xF:
			switch(cpu->IR.op_x.immediate){
				case 0x07:/* ld vx, dt */
					cpu->registers.V[cpu->IR.op_x.x] = cpu->delay_timer;
				break;
				case 0x0A:/* ld vx, k */
					;
					unsigned char pressed = 0x00; 
					for(unsigned char i = 0; i < 16; ++i) {
						if(cpu->keyboard->keybuffer[i] == 0x01) {
							cpu->registers.V[cpu->IR.op_x.x] = i;
							pressed = 0x01;
							break; 
						}
					}
					if(!pressed)
						cpu->PC -= 2;
				break;
				case 0x15:/* ld dt, vx */
					cpu->delay_timer = cpu->registers.V[cpu->IR.op_x.x];
				break;
				case 0x18:/* ld st, vx */
					cpu->sound_timer = cpu->registers.V[cpu->IR.op_x.x];
				break;
				case 0x1E:/* add i, vx */
					cpu->I += cpu->registers.V[cpu->IR.op_x.x];
				break;
				case 0x29:/* ld font, vx */
					cpu->I = cpu->registers.V[cpu->IR.op_x.x] * 5;
				break;
				case 0x33:/* ld bcd, vx */
					cpu->CHIP_8_RAM_WRITE(cpu, cpu->I + 0, cpu->registers.V[cpu->IR.op_x.x] / 100);
					cpu->CHIP_8_RAM_WRITE(cpu, cpu->I + 1, cpu->registers.V[cpu->IR.op_x.x] / 10 % 10);
					cpu->CHIP_8_RAM_WRITE(cpu, cpu->I + 2, cpu->registers.V[cpu->IR.op_x.x] % 10);
				break;
				case 0x55:/* ld [i], vx */
					for(unsigned char i = 0; i < cpu->IR.op_x.x + 1; ++i) {
						cpu->CHIP_8_RAM_WRITE(cpu, cpu->I + i, cpu->registers.V[i]);
					}
				break;
				case 0x65:/* ld vx, [i] */
					for(unsigned char i = 0; i < cpu->IR.op_x.x + 1; ++i) {
						cpu->registers.V[i] = cpu->CHIP_8_RAM_READ(cpu, cpu->I + i); 
					}
				break;
			}
		break;
	};
}

void cpu_init(struct chip_8_cpu *cpu, struct chip_8_memory *memory, struct chip_8_gpu *gpu, struct chip_8_keyboard *keyboard)
{
	cpu->memory = memory;
	cpu->gpu = gpu;
	cpu->keyboard = keyboard;
}

void cpu_reset(struct chip_8_cpu *cpu)
{
	cpu->IR.instruction = 0x0000;
	cpu->PC = 0x200;
	cpu->SP = 0x1F;
	
	memset(cpu->registers.V, 0x00, sizeof(cpu->registers.V));
	
	cpu->delay_timer = 0x00;
	cpu->sound_timer = 0x00;
}

void cpu_cycle(struct chip_8_cpu *cpu)
{
	fetch(cpu);
	decode(cpu);
}

