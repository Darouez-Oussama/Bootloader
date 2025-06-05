/**
 **************************************************************************************************
 *
 * @file    : bootloader_flash.c
 * @author  : Oussama DAROUEZ
 * @date    : August 2024
 * @brief   : Source file for Booting operations
 *
 **************************************************************************************************
 *
 * @project  : {In application programming}
 * @board    : {Nucleo_L476RG}
 * @compiler : {PlatformIO, EWARM}
 *
 **************************************************************************************************
 *
 */

/*-----------------------------------------------------------------------------------------------*/
/* Includes                                                                                      */
/*-----------------------------------------------------------------------------------------------*/
#include "bootloader.h"

/*-----------------------------------------------------------------------------------------------*/
/* Public methods definition                                                                     */
/*-----------------------------------------------------------------------------------------------*/

/**************************************************************************************************
  * @brief      boot to application partition
  * @return     nothing
  ********************************************************************************************** */
void boot (void)
{
  typedef void (*pFunction)(void);
  pFunction JumpToApplication;
  uint32_t JumpAddress;
  HAL_RCC_DeInit();
  HAL_DeInit();
  __set_MSP(*(__IO uint32_t*)APPLICATION_ADDRESS);
  uint32_t adr = *(__IO uint32_t*)(SWAP_ADDRESS);
  if (adr!=Empty)
  {
    copy_flash(SWAP_ADDRESS , APPLICATION_ADDRESS , APPLICATION_PAGE , SWAP_PAGE);
  }
  JumpAddress = *(__IO uint32_t*)(APPLICATION_ADDRESS + 4);
  JumpToApplication = (pFunction)JumpAddress;
  JumpToApplication();
}
