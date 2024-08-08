#include "../inc/chip_8_gpu.h"

void gpu_init(struct chip_8_gpu *gpu, struct chip_8_memory *memory, unsigned char width, unsigned char height)
{
	gpu->memory = memory;
	gpu->width = width;
	gpu->height = height;
}

void gpu_reset(struct chip_8_gpu *gpu)
{
	gpu->draw_frame_flag = 0;
}

void gpu_draw_dot(struct chip_8_gpu *gpu,unsigned char x, unsigned char y, color_t color)
{
	gpu->CHIP_8_VRAM_WRITE(gpu, gpu->width * (y / 8) + x, 0x01 << (y % 8));
}

void gpu_fill(struct chip_8_gpu *gpu, color_t color)
{
	
}

__attribute__((weak)) void gpu_render(struct chip_8_gpu *gpu)
{
	
}
