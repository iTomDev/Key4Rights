/*
 * Crypto_Storage.c
 *
 *  Created on: 29 Jul 2022
 *      Author: Tom
 *
 *
 *      This uses the onboard flash as a pseudo EEPROM. Not ideal due to flash write cycle limitations.
 *      This implementation uses the STM example so data is stored as predefine keypairs.
 */

//
#include <main.hpp>
#include "internal_storage.h"
#include "_core_system_config.h"
#include "eeprom.h"
#include "swd_tools.h"
#include "delay_tools.h"
#ifdef STM32H7
#include "stm32h7xx_hal_flash_ex.h"
#endif
#ifdef STM32F103
#include "stm32f1xx_hal_flash_ex.h"
#endif
#ifdef STM32F217
#include "stm32f2xx_hal_flash_ex.h"
#endif

#ifdef INTERNAL_STORAGE
#define FLASH_DEBUG
#define PWD_LENGTH	16

#define PWD1_0		0X0100
#define PWD1_1		0X0102
#define PWD1_2		0X0104
#define PWD1_3		0X0106
#define PWD1_4		0X0108
#define PWD1_5		0X010A
#define PWD1_6		0X010C
#define PWD1_7		0X010E

#define PWD1_8		0X0110
#define PWD1_9		0X0112
#define PWD1_10		0X0114
#define PWD1_11		0X0116
#define PWD1_12		0X0118
#define PWD1_13		0X011A
#define PWD1_14		0X011C
#define PWD1_15		0X011E

#define PWD2_0		0X0120
#define PWD2_1		0X0122
#define PWD2_2		0X0124
#define PWD2_3		0X0126
#define PWD2_4		0X0128
#define PWD2_5		0X012A
#define PWD2_6		0X012C
#define PWD2_7		0X012E

#define PWD2_8		0X0130
#define PWD2_9		0X0132
#define PWD2_10		0X0134
#define PWD2_11		0X0136
#define PWD2_12		0X0138
#define PWD2_13		0X013A
#define PWD2_14		0X013C
#define PWD2_15		0X013E

static void MPU_Config(void);


/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NB_OF_VAR] = {PWD1_0, PWD1_1, PWD1_2, PWD1_3, PWD1_4, PWD1_5, PWD1_6, PWD1_7};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue,VarDataTmp = 0;

void storage_init()
{
	MPU_Config();

	//HAL_FLASH_Unlock();

	if( EE_Init() != EE_OK)
	{
		#ifdef FLASH_DEBUG
		Flash_Error_Handler("err:setup ", (uint32_t) 10);
		#endif
	}
	//HAL_FLASH_Lock();
}

int store_pwd1(uint16_t *key)
{
	for(uint16_t i=0; i<PWD_LENGTH; i++)
	{
		if( (EE_WriteVariable(PWD1_0+(i*2), key[i]) != HAL_OK) )
		{
			#ifdef FLASH_DEBUG
			Flash_Error_Handler("err:write\n", (uint32_t) 10);
			#endif
		}
	}
}

int load_pwd1(uint16_t *key)
{
	for(uint16_t i=0; i<PWD_LENGTH; i++)
	{
		if( (EE_ReadVariable(PWD1_0+(i*2), &key[i]) != HAL_OK) )
		{
			#ifdef FLASH_DEBUG
			Flash_Error_Handler("err:read\n", (uint32_t) 9);
			#endif
		}
	}
}

int store_pwd2(uint16_t *key)
{
	for(uint16_t i=0; i<PWD_LENGTH; i++)
	{
		if( (EE_WriteVariable(PWD2_0+(i*2), key[i]) != HAL_OK) )
		{
			#ifdef FLASH_DEBUG
			Flash_Error_Handler("err:write\n", (uint32_t) 10);
			#endif
		}
	}
}

int load_pwd2(uint16_t *key)
{
	for(uint16_t i=0; i<PWD_LENGTH; i++)
	{
		if( (EE_ReadVariable(PWD2_0+(i*2), &key[i]) != HAL_OK) )
		{
			#ifdef FLASH_DEBUG
			Flash_Error_Handler("err:read\n", (uint32_t) 9);
			#endif
		}
	}
}

void erase_block(uint8_t sector)
{
	// erase sector
	uint32_t SectorError = 0;
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	//EraseInitStruct.Sector = FLASH_SECTOR_0; //Specify sector number
	EraseInitStruct.Sector = sector;
	EraseInitStruct.NbSectors = 1; //This is also important!
	if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {}

	// set default vals
	uint16_t testw[PWD_LENGTH];
	store_pwd1(&testw);
	store_pwd2(&testw);
}

void erase_block_default()
{
//#ifdef FLASH_BLOCK_ERASE
	HAL_FLASH_Unlock();

	/* Clear pending flags (if any) */
	  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
	                         FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR);

	// erase sector
	uint32_t SectorError = 0;
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = FLASH_SECTOR_2; //Specify sector number
	EraseInitStruct.NbSectors = (uint32_t) 1; //This is also important!
	if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {}

	// set default vals
	//uint16_t testw[PWD_LENGTH];
	//store_pwd1(&testw);
	//store_pwd2(&testw);
	HAL_FLASH_Lock();
//#endif

	// insecure overwrite
	uint16_t wipew[PWD_LENGTH] = {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}; // 0101
	//uint16_t wipew[PWD_LENGTH] = [0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55]; // 0101

	store_pwd1(&wipew);
	store_pwd2(&wipew);

	// shift from 01010101 to 10101010
	for(uint8_t i=0; i<PWD_LENGTH; i++)
	{
		wipew[i] = (wipew[i]<<1);
	}

	store_pwd1(&wipew);
	store_pwd2(&wipew);

	for(uint8_t i=0; i<PWD_LENGTH; i++)
	{
		wipew[i] = 0x00;
	}

	store_pwd1(&wipew);
	store_pwd2(&wipew);
}

void crypto_storage_demo()
{
	uint16_t testw[PWD_LENGTH];
	uint16_t testr[PWD_LENGTH];
	uint16_t testr2[PWD_LENGTH];
	uint8_t key1[16] = "ABCDEFGHIJKLMNOP";

	for(int i=0; i<PWD_LENGTH; i++)
	{
		testw[i] = key1[i];
	}

	HAL_FLASH_Unlock();

	storage_init();
	SWD_Write("I done\n", 7);

	while(1)
	{
		//HAL_FLASH_Unlock();
		// key1
		store_pwd1(&testw);
		HAL_Delay(100);
		SWD_Write(" W done\n", 8);
		load_pwd1(&testr);
		SWD_Write_Char16_Array(&testr, PWD_LENGTH);
		HAL_Delay(100);
		SWD_Write("\n", 1);
		HAL_Delay(10);
		SWD_Write(" R done\n", 8);

		// key2
		store_pwd2(&testw);
		HAL_Delay(100);
		SWD_Write(" W done\n", 8);
		load_pwd2(&testr);
		SWD_Write_Char16_Array(&testr,PWD_LENGTH);
		HAL_Delay(100);
		SWD_Write("\n", 1);
		HAL_Delay(10);
		SWD_Write(" R done\n", 8);
		HAL_FLASH_Lock();

		HAL_Delay(500);
		// erase
		SWD_Write("erase\n", 6);
		erase_block_default();
		//EE_Format_2();
		HAL_Delay(500);

		HAL_Delay(10);
		HAL_FLASH_Unlock();

		// verify erase
		load_pwd2(&testr2);
		SWD_Write_Char16_Array(&testr2,PWD_LENGTH);
		HAL_Delay(100);
		SWD_Write("\n", 1);
		HAL_Delay(10);
		SWD_Write(" R2 done\n", 9);
		HAL_FLASH_Lock();
		HAL_Delay(10);
	}
}




void Flash_Error_Handler(uint8_t *ptr, uint32_t len)
{
	SWD_Write(ptr, 11);
	delayms(500);
	return;
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
