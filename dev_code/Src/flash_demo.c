
//#include "flash_demo.h"



/*

uint8_t mode;
uint8_t key1[16]; // string of 8 characters
uint8_t key2[16];
uint8_t decoykey1[16];
uint8_t decoykey2[16];
uint8_t bulkkey[16]; // 128 bit key

// save the global values to flash
void Save_Crypto_Block()
{
	
	uint_t buff[];
	
	// combine into a single block
	for (uint8_t i=1; i<16; i++)
	{
		buff[i+0] = key1[i];
		buff[i+16] = key2[i];
		buff[i+32] = decoykey1[i];
		buff[i+48] = decoykey2[i];
		buff[i+64] = bulkkey[i];
	}
	
	// call block save
}

void Load_Crypto_Block()
{
	// key 1
	
	
	// save a 256 byte block. save it 4 times for redundancy
	
}
*/

/*
void flash_initialise()
{
	HAL_FLASH_Unlock();
}

void flash_deinitialise()
{
	HAL_FLASH_Lock();
}


uint8_t flash_write(volatile uint32_t* address, uint32_t *data, uint16_t size)
{
   for (int i = 0; i < size; i++)
     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (*address)+(i*4), data[i]) != HAL_OK)
    	return 1;
   return 0;
}



void flash_erase(uint8_t sector)
{
    uint32_t error = 0;
    FLASH_EraseInitTypeDef FLASH_EraseInitStruct =
    {
    	.TypeErase = FLASH_TYPEERASE_SECTORS,
	.Sector = (uint32_t)sector,
	.NbSectors = 1,
	.VoltageRange = FLASH_VOLTAGE_RANGE_3
    };

    HAL_FLASHEx_Erase(&FLASH_EraseInitStruct,&error);
}

uint8_t test_flash_access()
{

}
*/
