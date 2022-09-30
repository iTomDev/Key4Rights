/*
 * ataes132_spi.c
 *
 *  Created on: Sep 9, 2022
 *      Author: Tom

 -------------Checksum calculation-----------------
 This is a cause of a lot of problems with this device. Its best to precompute for our needs
 http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
 settings:
 CRC-16, CRC16_BUYPASS, byes
 is tested working
 The count value includes the count value itself and the checksum
 calculate the checksum including the count value and everything after it

 -----------------packet form----------------------
 (1) spi command
 (2:3) register address. either an address if its the eeprom, or 0xFE00 for the command exec register
 (4) count. includes this value and everything after it
 (5) opcode	code for the operation being executed
 (6) mode
 (7:8) param2
 (9:10) 0x0000
 (11:12) checksum

To access the command registers you need to write a command 0x02 then read from the 0xFE00 using 0x03. Weird.

 -------------hardware config---------------------
 + 4K7 on MISO and CS

 */

#include "stm32h7xx_hal.h"
#include "swd_tools.h"
#include "_core_system_config.h"
#include "cmsis_os2.h"
//#include "keystore.hpp"
//#include "RNG.hpp"
#include "ataes132_spi.hpp"
#include "spi_tools.hpp"

/***************************************************************************************
 *  Management
 */

// default constructor
ATAES132_SPI::ATAES132_SPI() {

}

// return 0 for success, -1 for fail or device not present
int8_t ATAES132_SPI::init( SPI_HandleTypeDef *hspiin, osMutexId_t mutexin) {
	*localhspi = *hspiin;
	this->atmutex = mutexin;
	return 0;
}

int8_t ATAES132_SPI::config_device(SPI_HandleTypeDef *hspi) {
	return 0;
}

void ATAES132_SPI::startRunning() {
	this->running = 1;
}

uint8_t ATAES132_SPI::getRunningState() {
	return this->running;
}



/***************************************************************************************
 * Private RNG
 */

int8_t ATAES132_SPI::at132s_lock_config(SPI_HandleTypeDef *hspi)
{
	uint8_t resp[10];
	uint8_t cmd[10] = { 	0x02,	// write
							0xFE, 	// reg addy
							0x00,	// reg
							0x07,	// count
							0x0D,	// opcode
							0x80,	// mode
							0x00,	// zone hi
							0x00,	// zone lo
							0x6F,	// checksum
							0x6B,	// checksum
	};
	spi_send(hspi, &cmd[0], &resp[0], 10);
	osDelay(10);
}

// check if config is locked
// 1 unlocked, 0 locked, -1 error
// reply: 0, 0, 0, 12, 0, 85,
int8_t ATAES132_SPI::at132s_get_lock_status(SPI_HandleTypeDef *hspi)
{
	uint8_t resp[12];
	uint8_t cmd[12] = { 	0x02,	// write
							0xFE, 	// reg addy
							0x00,	// reg
							0x09,	// count
							0x10,	// opcode
							0x00,	// mode
							0xF0,	// addy hi
							0x20,	// addy lo
							0x00,	// count
							0x08,	// count
							0x4B,	// check
							0x1A,	// sum
	};

	spi_send(hspi, &cmd[0], &resp[0], 12);
	osDelay(1);

	// read 16 byte random
	uint8_t cmdrd[6] = { 	0x03,	// read
							0xFE, 	// reg addy
							0x00,
							0x00,
							0x00,
							0x00
	};
	spi_send(hspi, &cmdrd[0], &resp[0],6 );
	osDelay(1);

	#ifdef SWD_DEBUG
	SWD_Write_Uint16_Array(&resp[0],6);
	osDelay(10);
	#endif

	// if locked
	if (resp[5] == 0x00)
	{
		return 0;
	}
	// if unlocked
	if (resp[5] == 0x55)
	{
		return 1;
	}
	// otherwise error
	return -1;
}

// 0, 0, 0, 12, 0, 65, 89, 18, 8, 144, 132, 42,
int8_t ATAES132_SPI::at132s_get_serial(SPI_HandleTypeDef *hspi, uint8_t *data)
{
	uint8_t resp[13];

	// send command - for SN
	// 0, 0, 0, 12, 0, 65, 89, 18, 8, 144, 132, 42, 207, 10,
	uint8_t cmd[12] = { 	0x02,	// write
							0xFE, 	// reg addy
							0x00,	// reg

							0x09,	// count
							0x10,	// opcode
							0x00,	// mode
							0xF0,	// addy hi
							0x00,	// addy lo
							0x00,	// count
							0x08,	// count
							0xC9,	// checksum
							0x99,	// checksum
	};
	spi_send(hspi, &cmd[0], &resp[0], 12);
	osDelay(10);

	// read
	uint8_t cmdrd[13] = { 	0x03,	// read
							0xFE, 	// reg addy
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00
	};
	spi_send(hspi, &cmdrd[0], &resp[0],13 );
	osDelay(10);

	#ifdef SWD_DEBUG
	SWD_Write_Uint16_Array(&resp[0],13);
	osDelay(10);
	#endif
	// copy out SN
	for (uint8_t i=0; i<8; i++)
	{
		*data++ = resp[i+6];
	}

	// otherwise error
	return -1;
}

// returns 16 byte random number
// reply when device is not locked:
// 0  1  2   3  4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
// 0, 0, 0, 20, 0, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165,
// (4) is the error code of 0x00
int8_t ATAES132_SPI::at132s_get_random(SPI_HandleTypeDef *hspi, uint8_t *data)
{
	// write enable = 0000 0110 =
	uint8_t cmd_wren = 0x06;
	uint8_t resp[21];// = {1,2,3,4,5,6,7,8, 9,10,11,12,13,14,15,16};

	spi_send(hspi, &cmd_wren, &resp[0], 1);
	//HAL_Delay(1);

	// send command
	uint8_t cmd[12] = { 0x02,	//	write cmd = 0000 0010 b
						0xFE, // reg addy
						0x00,
						0x09,	// first byte at FE00 is a count
						0x02,	// then opcode for random
						0x02, 	// mode
						0x00,	// param2a
						0x00,	// param2b
						0x00,
						0x00,
						0xF9,	// check
						0x60	// sum
	};

	spi_send(hspi, &cmd[0], &resp[0], sizeof(cmd) );
	osDelay(60);

	// read 16 byte random
	uint8_t cmdrd[11] = { 	0x03,	// read
							0xFE, 	// reg addy
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00,
							0x00
	};
	spi_send(hspi, &cmdrd[0], &resp[0],21 );
	osDelay(10);

	// extract random number from response
	for (uint8_t i=0; i<16; i++)
	{
		*data++ = resp[(i+6)];
	}
	#ifdef SWD_DEBUG
	SWD_Write_Uint16_Array(&resp[0],21);
	osDelay(10);
	#endif
}

/***************************************************************************************
 * Keystore private
 */

void ATAES132_SPI::at132s_data_store(SPI_HandleTypeDef *hspi, uint16_t memaddy, uint8_t *data, uint8_t len)
{
	uint8_t resp[35];

	// send write enable
	uint8_t cmd_wren = 0x06;
	spi_send(hspi, &cmd_wren, &resp[0], 1);
	osDelay(1);

	// form a write frame
	uint8_t cmd[35];
	cmd[0] = 0x02; 			// write
	cmd[1] = (memaddy << 8);		// hi
	cmd[2] = (uint8_t) memaddy; 	// lo
	for (uint8_t i=0; i<len; i++)
	{
		cmd[(i+3)] = *data++;
	}
	// send write
	spi_send(hspi, &cmd[0], &resp[0], 35 );
	osDelay(60);
}

void ATAES132_SPI::at132s_data_load(SPI_HandleTypeDef *hspi, uint16_t memaddy, uint8_t *data, uint8_t len)
{
	uint8_t resp[35];
	uint8_t cmdrd[35];

	// form commnd packet
	cmdrd[0] = 0x03;
	cmdrd[1] = (memaddy << 8);		// hi
	cmdrd[2] = (uint8_t) memaddy; 	// lo
	for (uint8_t i=0; i<len; i++)
	{
		cmdrd[i+3] = 0;
	}

	spi_send(hspi, &cmdrd[0], &resp[0], 35 );
	osDelay(10);

	// extract data from response
	for (uint8_t i=0; i<len; i++)
	{
		*data++ = resp[i+3];
	}
}


/***************************************************************************************
 * public rng
 */

void ATAES132_SPI::rtos_rng_update() {
	// if its there
	if(running == 1) {
		if(device_present > 0) {

			// generate random number if requested
			if(getRNGState()== 1) {
				// request
				uint8_t in[16];
				uint8_t out[16];
				// get two vectors of 16 random bytes so we can discard some
				// use in and out for memory efficiency, we can wipe out later anya
				at132s_get_random(localhspi, &in[0]);
				osDelay(50);
				at132s_get_random(localhspi, &out[0]);
				// loop through in and replace any over 247s with a spare from out
				uint8_t outj = 0; // track movement through out
				for (uint8_t i=0; i<16; i++)  {
					if( in[i] > 247)
					{
						while (outj < 16) {
							// check if under 248
							if( out[outj < 248])
							{
								// replace with spare
								in[i] = out[outj];
								break;
							}
							outj++;
						}
						if (outj > 15) {
							// exceeded number of spares available. its not impossible just unlikely
							// reject this iteration and run again
							setRNGState(1);
							return;
						}
					}
				}
				// zero out array
				for (uint8_t i=0; i< 16; i++) {
					out[i] = 0;
				}
				// convert
				uint8ToAlphanumericMap(&in[0], &out[0], 16);
				// copy to output buffer
				this->setGeneratedKey(&out[0], 16);
				// set task as done
				setRNGState(2);
			}// RNG requested

			// .... other features here

		}// if present
		// well it might be
		else {
			if( device_present < 0) {
				// no it really isn't
			}
			else {
				// try to setup
				if (config_device(localhspi) == 0){
					device_present = 0;
					config_status = 1;
					// verify device lock
					if(at132s_get_lock_status(localhspi) == 0) {
						lock_status = 1;
					}
				}
				else {
					// init failed
					device_present = -1;
				}
			}
		}// might be present
	}// running
}


/***************************************************************************************
 * Pulic Keystore
 */

// todo
void ATAES132_SPI::rtos_keystore_update() {
	if (running == 1) {
		if (rtos_key_to_get > 0) {
			// dont need to do anything
		}
		if (rtos_save_keys > 0) {
			// storage address
			uint16_t memaddy = 0;
			// key 1
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 1);
			at132s_data_store(localhspi, memaddy, &key1[0], 16);
			// key 2
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 2);
			at132s_data_store(localhspi, memaddy, &key2[0], 16);
			// key 3
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 3);
			at132s_data_store(localhspi, memaddy, &key3[0], 16);
			// key 4
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 4);
			at132s_data_store(localhspi, memaddy, &key4[0], 16);
			// key 5
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 5);
			at132s_data_store(localhspi, memaddy, &key5[0], 16);
			// key 6
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 6);
			at132s_data_store(localhspi, memaddy, &key6[0], 16);
			// key 7
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 7);
			at132s_data_store(localhspi, memaddy, &key7[0], 16);
			// key 8
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 8);
			at132s_data_store(localhspi, memaddy, &key8[0], 16);
			// key 9
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 9);
			at132s_data_store(localhspi, memaddy, &key9[0], 16);
		}
		if (rtos_load_keys > 0) {
			uint16_t memaddy = 0;
			// key 1
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 1);
			at132s_data_load(localhspi, memaddy, &key1[0], 16);
			// key 2
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 2);
			at132s_data_load(localhspi, memaddy, &key2[0], 16);
			// key 3
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 3);
			at132s_data_load(localhspi, memaddy, &key3[0], 16);
			// key 4
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 4);
			at132s_data_load(localhspi, memaddy, &key4[0], 16);
			// key 5
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 5);
			at132s_data_load(localhspi, memaddy, &key5[0], 16);
			// key 6
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 6);
			at132s_data_load(localhspi, memaddy, &key6[0], 16);
			// key 7
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 7);
			at132s_data_load(localhspi, memaddy, &key7[0], 16);
			// key 8
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 8);
			at132s_data_load(localhspi, memaddy, &key8[0], 16);
			// key 9
			memaddy = ATAES132_USER_MEMORY_BASE_ADDRESS + (ATAES132_USER_MEMORY_2K_OFFSET * 9);
			at132s_data_load(localhspi, memaddy, &key9[0], 16);
		}
	}
}




/***************************************************************************************
 * Unit Tests
 */


int8_t ates132s_test_config(SPI_HandleTypeDef *hspi, osMutexId_t mutexin)
{
	/*
	at132s_get_lock_status(hspi);

	uint8_t serialno[8] = {0,0,0,0,0,0,0,0};
	at132s_get_serial(hspi, &serialno[0]);
	*/
}

/**
 * Test getRandom
 * Device must be unlocked to run this test
 */
int8_t ates132s_test_get_random(SPI_HandleTypeDef *hspi)
{
	/*
	uint8_t rx[32] = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
	at132s_get_random(hspi, &rx[0]);
	for (uint8_t i=0; i<32; i++)
	{
		if (rx[i] != 165)
		{
			// error: data sent and received did not match
			return -1;
		}
	}
	*/
}

// -1 fail, 0 pass
int8_t ates132s_test_eeprom_save_and_load(SPI_HandleTypeDef *hspi)
{
	/*
	uint8_t tx[32] = {1,2,3,4,5,6,7,8, 9,10,11,12,13,14,15,16, 17,18,19,20,21,22,23,24, 25,26,27,28,29,30,31,32 };
	uint8_t rx[32] = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
	at132s_data_store(hspi, 0x0100, &tx[0], 32);
	at132s_data_load(hspi, 0x0100, &rx[0], 32);
	SWD_Write_Uint16_Array(&rx[0],32);
	osDelay(10);

	for (uint8_t i=0; i<32; i++)
	{
		if (tx[i] != rx[i])
		{
			// error: data sent and received did not match
			return -1;
		}
	}
	return 0;
	*/
}

