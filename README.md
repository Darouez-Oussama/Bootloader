# STM32L476RG Bootloader Project

## Overview
This is a bootloader project for the STM32L476RG microcontroller. A bootloader is a special program that runs before the main application and is responsible for initializing the system and loading/updating the main application firmware.

## Memory Layout
The STM32L476RG microcontroller features:
- 1024KB of Flash memory
- 96KB of SRAM (main RAM)
- 32KB of SRAM2 (additional RAM)

### Flash Memory Organization
The bootloader is located at the beginning of the flash memory, starting at address `0x8000000`. This is defined in the linker script (`ldscripts/STM32L476RGTX_FLASH.ld`):

```
FLASH (rx) : ORIGIN = 0x8000000, LENGTH = 1024K
```

This address is crucial because:
1. It's the default reset vector location where the STM32 starts executing code after reset
2. The bootloader must be placed here to take control before any other code executes

### Memory Map Details
- Flash Memory: 0x8000000 - 0x80FFFFF (1024KB)
- SRAM1 (main): 0x20000000 - 0x20017FFF (96KB)
- SRAM2: 0x10000000 - 0x10007FFF (32KB)

## Project Structure
The project uses the following memory configuration:
- Minimum Heap Size: 0x200 (512 bytes)
- Minimum Stack Size: 0x400 (1024 bytes)

### Important Sections in Linker Script
1. `.isr_vector`: Contains the vector table with interrupt handlers
2. `.text`: Contains the program code
3. `.data`: Contains initialized data
4. `.bss`: Contains uninitialized data
5. `._user_heap_stack`: Reserved for heap and stack

## Memory Protection
The bootloader's memory space should be protected from accidental erasure or overwriting. This can be achieved through:
- Read/write protection mechanisms
- Setting appropriate flash option bytes

## Notes
- The bootloader is placed at the start of flash memory (0x8000000)
- The main application should be linked to run from a different flash address to avoid conflicts
- Ensure proper memory boundaries and protections are set up

## Hardware Requirements
- NUCLEO-L476RG Board
- STM32L476RG microcontroller
- ST-LINK programmer (integrated on NUCLEO board)

## References
- STM32L476RG Reference Manual
- ST Programming Manual
- Application Note: AN2606 (STM32 microcontroller system memory boot mode)

## Boot Process and Memory Partitioning
The flash memory (1024KB total) is divided into three main regions:

| Region          | Start Address | End Address | Size   | Description                                    |
|----------------|---------------|-------------|--------|------------------------------------------------|
| Bootloader     | 0x8000000    | 0x8007FFF  | 32KB   | Contains bootloader code and vector table      |
| Application    | 0x8008000    | 0x8047FFF  | 256KB  | Main application firmware execution region     |
| Swap Region    | 0x8048000    | 0x8087FFF  | 256KB  | Temporary storage for firmware updates         |

### Boot Process Flow
1. System starts execution from bootloader region (0x8000000)
2. Bootloader checks the swap region (0x8048000) for new firmware:
   - If swap region is empty: Jump directly to application region
   - If swap region contains valid firmware:
     a. Copy firmware from swap region to application region
     b. Verify the copied firmware
     c. Clear the swap region
     d. Jump to application region

### Memory Usage Details
- **Bootloader Region (32KB)**
  - Contains bootloader code
  - Vector table
  - Firmware verification logic
  - Memory management functions

- **Application Region (256KB)**
  - Main firmware execution area
  - Gets updated when new firmware is present in swap region

- **Swap Region (256KB)**
  - Temporary storage for new firmware
  - Cleared after successful firmware update
  - Used during OTA or serial firmware updates

### Firmware Update Process
1. New firmware is received and stored in swap region
2. On next boot, bootloader detects new firmware in swap region
3. Bootloader copies firmware from swap to application region
4. Verification of copied firmware is performed
5. Swap region is cleared after successful update
6. System jumps to application region to execute new firmware 