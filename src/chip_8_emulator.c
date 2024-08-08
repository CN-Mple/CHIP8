#include "../inc/chip_8_emulator.h"

void emulator_init(struct chip_8_emulator *emulator)
{
	emulator->memory.reset(&emulator->memory);
	
	emulator->cpu.init(&emulator->cpu, &emulator->memory, &emulator->gpu, &emulator->keyboard);
	emulator->gpu.init(&emulator->gpu, &emulator->memory, CHIP_8_WIDTH, CHIP_8_HEIGHT);
	
	emulator->keyboard.reset(&emulator->keyboard);
}

void emulator_reset(struct chip_8_emulator *emulator)
{
	emulator->memory.reset(&emulator->memory);
	
	emulator->cpu.reset(&emulator->cpu);
	emulator->gpu.reset(&emulator->gpu);
	
	emulator->keyboard.reset(&emulator->keyboard);
}

__attribute__((weak)) void emulator_read(struct chip_8_emulator *emulator, unsigned short const *code, unsigned short size)
{
	
}

void emulator_run(struct chip_8_emulator *emulator)
{
    tick_t start_time = GET_TICK();
    tick_t cpu_last_time = start_time;
    tick_t display_last_time = start_time;

    while (1) {
        tick_t current_time = GET_TICK();
        double elapsed_cpu = (double)(current_time - cpu_last_time) / TICK_PER_SEC;
        double elapsed_display = (double)(current_time - display_last_time) / TICK_PER_SEC;

        // CPU cycle every CPU_CYCLE seconds
        if (elapsed_cpu > CPU_CYCLE) {
            cpu_last_time = current_time;
            emulator->cpu.cycle(&(emulator->cpu));
        }

        // Display update every DISPLAY_CYCLE second
        if (elapsed_display > DISPLAY_CYCLE) {
            display_last_time = current_time;

            if (emulator->gpu.draw_frame_flag == 0x1) {
                emulator->gpu.draw_frame_flag = 0x0;
                emulator->gpu.render(&(emulator->gpu));
            }

            emulator->keyboard.poll(&(emulator->keyboard));

            if (emulator->cpu.delay_timer > 0) {
                emulator->cpu.delay_timer--;
            }
            if (emulator->cpu.sound_timer > 0) {
                emulator->cpu.sound_timer--;
            }
        }
    }
}

