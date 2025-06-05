/**
 **************************************************************************************************
 *
 * @file    : bootloader_flash.h
 * @author  : Oussama DAROUEZ
 * @date    : August 2024
 * @brief   : Header file for Flash operations
 *
 **************************************************************************************************
 *
 * @project  : {In application programming}
 * @board    : {Nucleo_L476RG}
 * @compiler : {PlatformIO, EWARM}
 *
 **************************************************************************************************
 */

#ifndef INC_BOOTLOADER_FLASH_H_
#define INC_BOOTLOADER_FLASH_H_

/*-----------------------------------------------------------------------------------------------*/
/* Includes                                                                                      */
/*-----------------------------------------------------------------------------------------------*/
#include "main.h"

/*-----------------------------------------------------------------------------------------------*/
/* Defines                                                                                       */
/*-----------------------------------------------------------------------------------------------*/
#define APPLICATION_ADDRESS ((uint32_t)0x08040000)
#define SWAP_ADDRESS ((uint32_t) 0x08080000)
#define USER_FLASH_SIZE 497644
#define APPLICATION_PAGE 127
#define SWAP_PAGE 256

/*-----------------------------------------------------------------------------------------------*/
/* public functions declaration                                                                  */
/*-----------------------------------------------------------------------------------------------*/
void flash_erase(uint32_t page , uint32_t bank , uint32_t nbPages );
void copy_flash(uint32_t swapAddress , uint32_t applicationAddress , uint32_t applicationPage , uint32_t swapPage) ;

#endif /* INC_BOOTLOADER_FLASH_H_ */
