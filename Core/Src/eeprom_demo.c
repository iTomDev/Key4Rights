/*
 * eeprom_demo.c
 *
 *  Created on: Jul 25, 2022
 *      Author: Tom
 */

#include <main.hpp>
#include "eeprom.h"
#include "eeprom_demo.h"
#include "swd_tools.h"



#ifdef USE_EEPROM_DEMO

// sim EEPROM dev
FLASH_OBProgramInitTypeDef OBInit;

/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue,VarDataTmp = 0;

/* Private function prototypes -----------------------------------------------*/
//static void SystemClock_Config(void);
//static void Error_Handler(void);
//static void CPU_CACHE_Enable(void);
static void MPU_Config(void);

void eeprom_demo_init()
{
	MPU_Config();

	HAL_FLASH_Unlock();

	if( EE_Init() != EE_OK)
	{
		//Error_Handler();
		uint8_t debugstr[10] = "err:setup ";
		SWD_Write(&debugstr, (uint32_t)sizeof(debugstr));
		HAL_Delay(500);

		//uint8_t debugstr[1];
		debugstr[0] = EE_Init();
		SWD_Write_Uint8(debugstr[0], 1);
		HAL_Delay(500);
		return;
	}
}

void eeprom_demo()
{
	/* Store 0x1000 values of Variable1 in EEPROM */
	for (VarValue = 1; VarValue <= 0x1000; VarValue++) // about 2 sec to write 4KB?
	{
		/* Sequence 1 */
		if((EE_WriteVariable(VirtAddVarTab[0],  VarValue)) != HAL_OK)
		{
			uint8_t debugstr[10] = "err:write ";
			SWD_Write(&debugstr, (uint32_t)sizeof(debugstr));
			HAL_Delay(500);
			//Error_Handler();
			return;
		}
		if((EE_ReadVariable(VirtAddVarTab[0],  &VarDataTab[0])) != HAL_OK)
		{
			//Error_Handler();
			uint8_t debugstr[10] = "err:read  ";
			SWD_Write(&debugstr, (uint32_t)sizeof(debugstr));
			HAL_Delay(500);
			return;
		}
		if (VarValue != VarDataTab[0])
		{
			//Error_Handler();
			uint8_t debugstr[10] = "err:valid ";
			SWD_Write(&debugstr, (uint32_t)sizeof(debugstr));
			HAL_Delay(500);
			return;
		}
	}
	uint8_t debugstr[10] = "eeprom ok ";
	SWD_Write(&debugstr, (uint32_t)sizeof(debugstr));
	HAL_Delay(500);
}


/**
  * @brief  Configure the MPU attributes as Normal Non Cacheable for Flash Bank B sectors 6 and 7 (used for EEPROM emulation).
  * @note   The Base Address is 0x081C0000 .
  *         The Configured Region Size is 256KB (2 x 128 KB for 2 flash sectors).
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes as Normal Non Cacheable for Flash Bank B sectros 6 and 7 */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = ADDR_FLASH_SECTOR_6_BANK2;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}
#endif
