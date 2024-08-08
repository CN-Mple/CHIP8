**This is a Simple CHIP8 emulator written in C.**

This project is based on the "DEV" platform using the console window as a black and white display( If your DEV is ready, you can compile directly to F11 ).

CHIP8 is the oldest simulator born in 1970, it has 35 opcodes, I wanted to increase its portability, so I chose to use C to implement it🤣.

# Board interface

## Display Device
- **Windows Control console.**

## Keyboard interface
**Mapping relationship**
|        | 1   | 2   | 3   | 4   |  
|--------|-----|-----|-----|-----|  
| **1**  | 1   | 2   | 3   | 4   |  
| **2**  | Q   | W   | E   | R   |  
| **3**  | A   | S   | D   | F   |  
| **4**  | Z   | X   | C   | V   |  


|        | 1   | 2   | 3   | 4   |  
|--------|-----|-----|-----|-----|  
| **1**  | 1   | 2   | 3   | C   |  
| **2**  | 4   | 5   | 6   | D   |  
| **3**  | 7   | 8   | 9   | E   |  
| **4**  | A   | 0   | B   | F   |  
- **keyboard_poll**

## Tick interface
- **get_tick**
> Use for CPU and GPU ticks (Unit ms).

## CPU interface
- **memory_reset**
> User clear memory data.
- **memory_write**
> User write memory data.
- **memory_read**
> User read memory data.
- **emulator_read**
> User read rom code to memory.

## GPU interface
- **gpu_render**
> Use for updata to Display.
