/**
 **************************************************************************************************
 *
 * @file    : bootloader_flash.c
 * @author  : Oussama DAROUEZ
 * @date    : August 2024
 * @brief   : Source file for Flash operations
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
#include "bootloader_flash.h"
/*-----------------------------------------------------------------------------------------------*/
/* Macro                                                                                      */
/*-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*/
/* Private variables                                                                             */
/*-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*/
/* Private methods declaration                                                                   */
/*-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*/
/* Public methods definition                                                                     */
/*-----------------------------------------------------------------------------------------------*/

/**************************************************************************************************
  * @brief      erase from flash
  * @param      page     desired page number
  * @param      bank     bank
  * @param      nbPages  number of pages to be erased
  * @return     nothing
  ********************************************************************************************** */
void flash_erase(uint32_t page , uint32_t bank , uint32_t nbPages)
{
  FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t erase_flash_status;
  HAL_FLASH_Unlock();
  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks        = bank;
  EraseInitStruct.Page     = page;
  EraseInitStruct.NbPages  = nbPages;
  HAL_FLASHEx_Erase(&EraseInitStruct, &erase_flash_status);
  HAL_FLASH_Lock();
}

/**************************************************************************************************
  * @brief      copy from flash to flash
  * @param      swapAddress            swap partition starting address
  * @param      applicationAddress     application partition starting address
  * @param      applicationPage        application partition page
  * @param      swapPage               swap partition page
  * @return     nothing
  ********************************************************************************************** */
void copy_flash(uint32_t swapAddress , uint32_t applicationAddress , uint32_t applicationPage , uint32_t swapPage  )
{
  uint32_t i = 0 ;
  flash_erase(applicationPage, FLASH_BANK_1 , 128);
  HAL_FLASH_Unlock();
  uint64_t data;
  while(1)
  {
    data = *(__IO uint64_t * )(swapAddress+i) ;
	if(data==0xFFFFFFFFFFFFFFFF) break ;
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, applicationAddress+i, data) ;
	i+=8;
  }
  HAL_FLASH_Lock();
  flash_erase(swapPage, FLASH_BANK_2 , 128);
}

