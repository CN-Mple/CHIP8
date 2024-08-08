#ifndef __CHIP_8_GPU_H
#define __CHIP_8_GPU_H

#include <stdio.h>
#include <string.h>

#include "./chip_8_memory.h"
#include "../board/chip_8_board.h"

typedef unsigned char	color_t;

struct chip_8_gpu {
	struct chip_8_memory *memory;
	
	unsigned char draw_frame_flag :1;
	unsigned char width;
	unsigned char height;
	
	void (*init)(struct chip_8_gpu *gpu, struct chip_8_memory *memory, unsigned char width, unsigned char height);
	void (*reset)(struct chip_8_gpu *gpu); 
	void (*draw_dot)(struct chip_8_gpu *gpu,unsigned char x, unsigned char y, color_t color);
	void (*fill)(struct chip_8_gpu *gpu, color_t color);
	void (*render)(struct chip_8_gpu *gpu);
};

void gpu_init(struct chip_8_gpu *gpu, struct chip_8_memory *memory, unsigned char width, unsigned char height);
void gpu_reset(struct chip_8_gpu *gpu); 
void gpu_draw_dot(struct chip_8_gpu *gpu,unsigned char x, unsigned char y, color_t color);
void gpu_fill(struct chip_8_gpu *gpu, color_t color);
void gpu_render(struct chip_8_gpu *gpu);

#define default_struct_gpu()	{					\
									0,				\
									0,				\
									64,				\
									32,				\
									gpu_init,		\
									gpu_reset,		\
									gpu_draw_dot,	\
									gpu_fill,		\
									gpu_render		\
								}

#endif
