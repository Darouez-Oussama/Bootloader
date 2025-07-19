# STM32 Bootloader Project

This project implements a bootloader for the STM32L476RG microcontroller, designed to manage the boot process and facilitate firmware updates by handling the transition between the bootloader, application, and swap regions.

## Boot Process and Memory Partitioning

The flash memory (1024KB total) is divided into three main regions:

| Region          | Start Address | End Address | Size   | Description                                    |
|-----------------|---------------|-------------|--------|------------------------------------------------|
| Bootloader      | 0x08000000    | 0x08007FFF  | 32KB   | Contains bootloader code and vector table, starting at the base flash address |
| Application     | 0x08008000    | 0x08047FFF  | 256KB  | Main application firmware execution region     |
| Swap Region     | 0x08048000    | 0x08087FFF  | 256KB  | Temporary storage for firmware updates         |

### Boot Process Flow

1. **System Startup**: Execution begins at the reset handler in the bootloader region (`0x08000000`), which is the default entry point after a reset as defined by the STM32's vector table.
2. **Bootloader Check**: The `boot` function in `bootloader.c` is invoked:
   - Deinitializes the RCC and HAL to reset peripheral states.
   - Sets the main stack pointer (MSP) to the application’s stack pointer at `APPLICATION_ADDRESS` (0x08008000).
   - Checks the swap region (`SWAP_ADDRESS`, 0x08048000) for new firmware by verifying it is not all `0xFF` (empty flash).
   - If the swap region contains valid firmware:
     a. Copies the firmware from the swap region to the application region using `copy_flash`.
     b. Verifies the copied firmware (implicitly assumed valid if present).
     c. Clears the swap region with `flash_erase`.
     d. Jumps to the application’s reset handler.
   - If the swap region is empty, jumps directly to the application’s reset handler.

## Implementation Details

### Bootloader Start Address

The bootloader is designed to start at `0x08000000`, the base address of the flash memory on the STM32L476RG. This address is set as the vector table base in the linker script and system configuration, ensuring the microcontroller begins execution from the bootloader after a reset or power-on.

### Bootloader and Flash Functions

The `bootloader.c` and `bootloader_flash.c` files contain the core logic for the boot process and flash operations:

- **`boot(void)` (bootloader.c)**:
  - **Purpose**: Manages the boot process, including the jump to the application.
  - **Process**:
    1. Calls `HAL_RCC_DeInit()` and `HAL_DeInit()` to reset peripheral states.
    2. Sets the MSP to the value at `APPLICATION_ADDRESS` (0x08008000), which is the application’s stack pointer from its vector table.
    3. Checks the swap region (`SWAP_ADDRESS`, 0x08048000) for valid firmware by reading the first 64-bit word; if not all `0xFF`, proceeds with the update.
    4. If valid firmware is detected, calls `copy_flash` to transfer data from the swap region to the application region.
    5. Uses a function pointer (`JumpToApplication`) to jump to the application’s reset handler, located at `JumpAddress` (offset 4 from `APPLICATION_ADDRESS`, the reset vector).
  - **Usage**: Called in `main.c` after a 2-second delay to allow initial setup.

- **`flash_erase(uint32_t page, uint32_t bank, uint32_t nbPages)` (bootloader_flash.c)**:
  - **Purpose**: Erases a specified number of pages in the flash memory.
  - **Parameters**:
    - `page`: The starting page number.
    - `bank`: The flash bank (e.g., `FLASH_BANK_1` for the application region, `FLASH_BANK_2` for the swap region).
    - `nbPages`: Number of pages to erase.
  - **Process**: Unlocks the flash, configures the `FLASH_EraseInitTypeDef` structure, erases the specified pages, and locks the flash. The status is stored but not returned.
  - **Usage**: Called in `copy_flash` to prepare the application and swap regions.

- **`copy_flash(uint32_t swapAddress, uint32_t applicationAddress, uint32_t applicationPage, uint32_t swapPage)` (bootloader_flash.c)**:
  - **Purpose**: Copies firmware from the swap region to the application region.
  - **Parameters**:
    - `swapAddress`: Starting address of the swap region (0x08048000).
    - `applicationAddress`: Starting address of the application region (0x08008000).
    - `applicationPage`: Page number of the application region.
    - `swapPage`: Page number of the swap region.
  - **Process**:
    1. Erases the application region with `flash_erase` to prepare for new data.
    2. Unlocks the flash and reads 64-bit data from the swap region.
    3. Programs the data to the application region in 64-bit chunks until an all-`0xFF` value is encountered, indicating the end of valid data.
    4. Locks the flash and erases the swap region to clear the temporary firmware.
  - **Usage**: Called in `boot` to update the application region with new firmware.

### Function Pointer for Jump

The `boot` function uses a function pointer (`pFunction JumpToApplication`) to perform the jump to the application. This is implemented as follows:
- A `typedef void (*pFunction)(void)` defines the function pointer type.
- `JumpAddress` is set to the value at `APPLICATION_ADDRESS + 4`, which is the reset vector (the address of the application’s reset handler) from the application’s vector table.
- `JumpToApplication` is cast to this address and invoked, ensuring a safe transition to the application context by preserving the call stack and register state.
