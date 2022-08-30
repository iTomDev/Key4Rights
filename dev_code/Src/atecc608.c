/*
 * atecc608b.c
 *
 *  Created on: Aug 16, 2022
 *      Author: Tom
 */

// I2C settings: 1Mhz

// app
#include "i2c_tools.h"
#include "atecc608.h"

// api
#ifdef STM32H7
#include "stm32h7xx_hal.h"
#endif
#ifdef STM32F103
#include "stm32f1xx_hal.h"
#endif
#ifdef STM32F217
#include "stm32f2xx_hal.h"
#endif


#define AT608_WRITE_CONFIG_ZONE
#define AT608_ADDRESS_CONFIG_ZONE_CONFIG_LOCK_VALUE		87		// return 0x00 if config is lock, 0x55 unlocked
#define AT608_AES_MODE 				0x00
#define AT608_AES_KEY_SLOT_9		0x09


/**
 *
 * RNG will produce a predefined value: 0xFF FF 00 00 FF FF 00 00
*/

/*
 * Init command which permanently locks the device so RNG etc can be used for real
 *
 */
int8_t i2c_ATECC608B_init()
{
	if(i2c_ATECC608B_detect() < 0){return AT608_NOT_DETECTED;}
	else
	{

	// set up config zone


	// aes enable
	// [13] = 0x01

	// counter 0
	// [52:59]

	// counter 1
	// [60:67]

	// slot config
	// slot0 needs to be config for AES
	// [20:51]

	// use lock
	// [68]

	// key config
	// [96:127]

	// if testing, dont lock the chip
	#ifndef LIVE_UNIT_TESTING
	// finally lock the config zone
	// 0: 0x17		lock command
	// 1: 0x01		mode. zone/slot to lock. bit 0 set to not do the crc compare. locks config zone
	// 2: 0x00
	// 3: 0x00
	// 4: 0x00
	// returns 0x00 on success
	#endif
	}
}

void i2c_ATECC608B_detect()
{
	return 0;
}

// identify what operations are supported
void i2c_ATECC608B_get_version()
{

}

void i2c_ATECC608B_storage_setup()
{
	// slot config
	// 6:	io protection key
	// 8:	general data
	// 9: 	aes key. write only. updateable. can store 4 keys

	// set up config and key bytes for each slot
	// use slot 8 as the others are mostly for cryto operations. 416 bytes
}

void i2c_ATECC608B_config_param_write()
{
	// ADDRESS ENCODING for config zone
	// byte 1 = 0x00
	// byte 0: 000 bb ooo // NA, block, offset

	// 0: 0x12		write
	// 1: AT608_WRITE_CONFIG_ZONE 0x00
	// 2:3: address
	// 4:... data
}

/*************************************************************************
 * AES
 */

void i2c_ATECC608B_aes_ecb_setup()
{
	// set the key in encrypt key slot
	// use the 4 byte write mode
	// read back key to verify

}

void i2c_ATECC608B_aes_ecb_encrypt()
{
	// start, send
	// 0: 0x51		opcode
	// 1: 0xmm		mode
	// 2:3: 0xkkk_	keyID, which memory slot
	// 4:19: 		16 bytes data to encrypt
	// repeated start, read
	// 0: 			success 0x00 or error code
	// 1:17			ciphertext, 16 bytes
	int8_t problems = 0;

	/*
	uint8_t txtmp[19];
	txtmp[0] = AT608_AES_MODE;
	txtmp[1] = 0x00;
	txtmp[2] = AT608_AES_KEY_SLOT_9;
	uint8_t rxtmp[16];

	problems = i2c1_write_then_read(AT608_DEVICE_ADDRESS, &txtmp, 0x05, &rxtmp, 16);
	*/
	return problems;
}

void i2c_ATECC608B_aes_ecb_decrypt()
{

}

/*************************************************************************
 * RNG
 */

// 32B buff
// todo doesnt this take 1ms though? clock stretching?
int8_t i2c_ATECC608B_get_random(uint8_t *rand)
{
	int8_t problems = 0;
	
	// if testing, dont lock the chip
	uint8_t txtmp[5] = {AT608_CMD_RANDOM, 
	0x00,
	0x00,
	0x00,
	0x00};
	uint8_t rxtmp[33];
	
	// get 32 byte rnd from device
	problems = i2c1_write_then_read(AT608_DEVICE_ADDRESS, &txtmp, 0x05, &rxtmp, 33);

	// copy 32 msB into rand
	for(uint8_t i=0; i<32; i++)
	{
		*rand++ = rxtmp[i];
	}

	// RNG will return default value (insecure!)
	#ifdef LIVE_UNIT_TESTING
	// check RNG error flags
	if(rxtmp[32] != 0x00){problems=-2;}
	// compare to test vector
	uint8_t RNGTest[8] = {0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00};
	for(uint8_t i=0; i<8; i++)
	{
		if (RNGTest[i] != rxtmp[i])
		{
			problems = -2;
			// test failed
		
			#ifdef SWD_DEBUG
			const uint8_t debugstr[15] = " RNG Test Fail ";
			SWD_Write(&debugstr, (uint32_t)sizeof(debugstr));
			#endif
		}
	}
	#else // for real
	// basic quality check
	uint16_t rngsum = 0;
	for (uint8_t i=0; i<32; i++)
	{
		// sum to find average. shouldn't be 0x00 or 0xFF
		rngsum += rxtmp[i];
		rngsum /= rngsum;
	}
	if(rngsum==0x00 || rngsum==0xFF)
	{
		// nothx :]
		return NOTHXKGB;
	}
	// check if the test vector is returned for RNG i.e config not locked

	#endif

return problems;
}



/*************************************************************************
 * Generic data storage
 */

/**
 * This is the generic data storage area
 * Slot 8, 416B
 * @param blockno 0-12
 * @param data pointer
 * @param blklen numnber of blocks to write
 * @return -n for error, 0 success
 */
// untested
int8_t i2c_ATECC608B_storage_save_multiblock(uint8_t blockno, uint8_t *data, uint8_t blkcnt)
{
	// slot 8
	// read command
	// 0: 0x12 read <--- sent separately
	// 1: 0x82 32Byte data zone read
	// 2: AT608_SLOT0_BLOCK1_H
	// 3: AT608_SLOT0_BLOCK1_L
	// 4:35... 32 Byte
	// 36: receive 1 byte result. 0x00 = success

	// temp data buff
	uint8_t tmp[35];

	// how many 32 byte blocks
	for(uint8_t i=0; i<blkcnt; i++)
	{
		tmp[0] = AT608_DATA_ZONE_32BYTE;
		tmp[1] = (AT608_SLOT8_BLOCKX_H) + blockno + i;
		tmp[2] = AT608_SLOT8_BLOCKX_L;
		// copy data into a new array but with config params at start
		for(uint8_t j=0; j<32; j++)
		{
			// offset as multiple blocks of 32 bytes to shift through
			tmp[j+3] = *data++;
		}

		i2c1_send_block(AT608_DEVICE_ADDRESS, AT608_CMD_WRITE, &tmp, 35);
	}
	return 0;
}

int8_t i2c_ATECC608B_storage_save(uint8_t blockno, uint8_t *data)
{
	// slot 8
	// read command
	// 0: 0x12 read <--- sent separately
	// 1: 0x82 32Byte data zone read
	// 2: AT608_SLOT0_BLOCK1_H
	// 3: AT608_SLOT0_BLOCK1_L
	// 4:35... 32 Byte
	// 36: receive 1 byte result. 0x00 = success

	if (blockno <13)
	{
		// temp data buff
		uint8_t tmp[35];

		tmp[0] = AT608_DATA_ZONE_32BYTE;
		tmp[1] = (AT608_SLOT8_BLOCKX_H) + blockno;
		tmp[2] = AT608_SLOT8_BLOCKX_L;
		// copy data into a new array but with config params at start
		for(uint8_t j=0; j<32; j++)
		{
			// offset as multiple blocks of 32 bytes to shift through
			tmp[j+3] = *data++;
		}

		i2c1_send_block(AT608_DEVICE_ADDRESS, AT608_CMD_WRITE, &tmp, 35);
		return 0;
	}
}

int8_t i2c_ATECC608B_storage_load(uint8_t blockno, uint8_t *data)
{
	// temp data buff
	uint8_t tmptx[4];

	tmptx[0] = AT608_CMD_READ;
	tmptx[1] = AT608_DATA_ZONE_32BYTE;
	tmptx[2] = (AT608_SLOT8_BLOCKX_H) + blockno;
	tmptx[3] = AT608_SLOT8_BLOCKX_L;

	int8_t err = i2c1_write_then_read(AT608_DEVICE_ADDRESS, &tmptx, (uint16_t) 4, data, 32);
	if (err == 0)
	{
		return 0;
	}
	else
	{
		// blockno too high
		return -3;
	}
}

/*************************************************************************
 * Specific data slots
 *
 */

// 326 Bytes: 0-6
void i2c_ATECC608B_36B_save(uint8_t slotno)
{

	// read command
	// 0: 0x12 read
	// 1: 0x82 32Byte data zone read
	// 2: AT608_SLOT0_BLOCK1_H
	// 3: AT608_SLOT0_BLOCK1_L
	// 4:35... 32 Byte
	// 36: receive 1 byte result. 0x00 = success
}

void i2c_ATECC608B_36B_read(uint8_t slotno)
{
	ASSERT(slotno < 7);

	// read command
	// 0: 0x02 read
	// 1: 0x82 32Byte data zone read
	// 2: AT608_SLOT0_BLOCK1_H
	// 3: AT608_SLOT0_BLOCK1_L
	// read out the data
	//

	// check status
	// 0x00 success
	// 0x0F exec error
}

// test functions











