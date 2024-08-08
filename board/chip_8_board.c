#include "chip_8_board.h"

/* gpu */
void gpu_render(struct chip_8_gpu *gpu) {
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    static CHAR_INFO buffer[64 * 32];
    static BOOL isFirstRender = TRUE;

    if (isFirstRender) {
        CONSOLE_CURSOR_INFO cci;
        cci.bVisible = FALSE;
        cci.dwSize = 1;
        SetConsoleCursorInfo(hOutput, &cci);
        isFirstRender = FALSE;
    }
    
	memset(buffer, 0, sizeof(buffer));
	
    for (int y = 0; y < gpu->height; ++y) {
        for (int x = 0; x < gpu->width; ++x) {
            CHAR_INFO cell;
            cell.Char.UnicodeChar = (gpu->CHIP_8_VRAM_READ(gpu, gpu->width * (y / 8) + x) & (0x01 << (y % 8))) ? '=' : ' ';
            cell.Attributes = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;
            buffer[x + y * gpu->width] = cell;
        }
    }
	
    COORD bufferSize = {64, 32};
    COORD bufferCoord = {0, 0};
    SMALL_RECT consoleWriteArea = {0, 0, 63, 31};
	
	
    WriteConsoleOutput(hOutput, buffer, bufferSize, bufferCoord, &consoleWriteArea);
}

/* keyboard */
static void key_press_deal(struct chip_8_keyboard *keyboard, char c, unsigned char ispressed)
{
	if(ispressed) {
		switch(c) {
	        	case '1':
	        		keyboard->keybuffer[0x1] = 0x0001;
	        	break;
	        	case '2':
	        		keyboard->keybuffer[0x2] = 0x0001;
	        	break;
	        	case '3':
	        		keyboard->keybuffer[0x3] = 0x0001;
	        	break;
	        	case '4':
	        		keyboard->keybuffer[0xC] = 0x0001;
	        	break;
	        	case 'q':
	        		keyboard->keybuffer[0x4] = 0x0001;
	        	break;
	        	case 'w':
	        		keyboard->keybuffer[0x5] = 0x0001;
	        	break;
	        	case 'e':
	        		keyboard->keybuffer[0x6] = 0x0001;
	        	break;
	        	case 'r':
	        		keyboard->keybuffer[0xD] = 0x0001;
	        	break;
	        	case 'a':
	        		keyboard->keybuffer[0x7] = 0x0001;
	        	break;
	        	case 's':
	        		keyboard->keybuffer[0x8] = 0x0001;
	        	break;
	        	case 'd':
	        		keyboard->keybuffer[0x9] = 0x0001;
	        	break;
	        	case 'f':
	        		keyboard->keybuffer[0xE] = 0x0001;
	        	break;
	        	case 'z':
	        		keyboard->keybuffer[0xA] = 0x0001;
	        	break;
	        	case 'x':
	        		keyboard->keybuffer[0x0] = 0x0001;
	        	break;
	        	case 'c':
	        		keyboard->keybuffer[0xB] = 0x0001;
	        	break;
	        	case 'v':
	        		keyboard->keybuffer[0xF] = 0x0001;
	        	break;
		}
	}else {
		switch(c) {
	        	case '1':
	        		keyboard->keybuffer[0x1] = 0x0000;
	        	break;
	        	case '2':
	        		keyboard->keybuffer[0x2] = 0x0000;
	        	break;
	        	case '3':
	        		keyboard->keybuffer[0x3] = 0x0000;
	        	break;
	        	case '4':
	        		keyboard->keybuffer[0xC] = 0x0000;
	        	break;
	        	case 'q':
	        		keyboard->keybuffer[0x4] = 0x0000;
	        	break;
	        	case 'w':
	        		keyboard->keybuffer[0x5] = 0x0000;
	        	break;
	        	case 'e':
	        		keyboard->keybuffer[0x6] = 0x0000;
	        	break;
	        	case 'r':
	        		keyboard->keybuffer[0xD] = 0x0000;
	        	break;
	        	case 'a':
	        		keyboard->keybuffer[0x7] = 0x0000;
	        	break;
	        	case 's':
	        		keyboard->keybuffer[0x8] = 0x0000;
	        	break;
	        	case 'd':
	        		keyboard->keybuffer[0x9] = 0x0000;
	        	break;
	        	case 'f':
	        		keyboard->keybuffer[0xE] = 0x0000;
	        	break;
	        	case 'z':
	        		keyboard->keybuffer[0xA] = 0x0000;
	        	break;
	        	case 'x':
	        		keyboard->keybuffer[0x0] = 0x0000;
	        	break;
	        	case 'c':
	        		keyboard->keybuffer[0xB] = 0x0000;
	        	break;
	        	case 'v':
	        		keyboard->keybuffer[0xF] = 0x0000;
	        	break;
		}
	} 
}

void keyboard_poll(struct chip_8_keyboard *keyboard)
{
    static HANDLE hStdin;
    static INPUT_RECORD irInBuf[128];
    DWORD fdwMode, fdwSaveOldMode;
    int cRecordsRead;

    hStdin = GetStdHandle(STD_INPUT_HANDLE);

    GetConsoleMode(hStdin, &fdwSaveOldMode);

    fdwMode = fdwSaveOldMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    SetConsoleMode(hStdin, fdwMode | ENABLE_WINDOW_INPUT);

    if (PeekConsoleInput(hStdin, irInBuf, 128, (LPDWORD)&cRecordsRead)) {
        if (cRecordsRead > 0) {
            if (!ReadConsoleInput(hStdin, irInBuf, 128, (LPDWORD)&cRecordsRead)) {
                fprintf(stderr, "Error: Unable to read console input.\n");
            }

            for (int i = 0; i < cRecordsRead; i++) {
                if (irInBuf[i].EventType == KEY_EVENT) {
                    key_press_deal(keyboard, irInBuf[i].Event.KeyEvent.uChar.AsciiChar, irInBuf[i].Event.KeyEvent.bKeyDown);
                }
            }
        }
    }
    SetConsoleMode(hStdin, fdwSaveOldMode);
}

/* memory */

struct chip_8_mem_ram ram;

void memory_reset(struct chip_8_memory *memory)
{
	memset(ram.data, 0, sizeof(ram.data));
    memcpy(ram.system_reserved.font_bitmap, font_data, sizeof(font_data));
    memcpy(ram.data, font_data, sizeof(font_data));
}

void memory_write(struct chip_8_memory *memory, unsigned short addr, unsigned char byte)
{
	(void)memory;
	ram.data[addr] = byte;
}

unsigned char memory_read(struct chip_8_memory *memory, unsigned short addr)
{
	return ram.data[addr];
}

void emulator_read(struct chip_8_emulator *emulator, unsigned short const *code, unsigned short size)
{
	(void)emulator;
	memcpy(ram.code.data, code, size);
}

/* tick */

tick_t get_tick(void)
{
	FILETIME ft;
    ULARGE_INTEGER li;

    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    ULONGLONG microseconds = li.QuadPart / 10;
    
    return microseconds;
}

