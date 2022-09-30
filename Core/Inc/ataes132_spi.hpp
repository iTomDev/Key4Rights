/*
 * ataes132_spi.hpp
 *
 *  Created on: Sep 15, 2022
 *      Author: Tom
 */

#ifndef INC_ATAES132_SPI_HPP_
#define INC_ATAES132_SPI_HPP_

#include "rngdev.hpp"
#include "keystore.hpp"
#include "ataes132_spi.hpp"
//sys
#include "stm32h7xx_hal.h"
#include "cmsis_os2.h"

#define ATAES132_USER_MEMORY_BASE_ADDRESS		0x0000
#define ATAES132_USER_MEMORY_2K_OFFSET			0x0100

class ATAES132_SPI : public Keystore, public RNGDEV {
private:
	SPI_HandleTypeDef *localhspi;
	uint8_t running;
	osMutexId_t atmutex;
	// RNG
	uint8_t device_present; // 1 present
	uint8_t lock_status; // 1 locked
	uint8_t config_status; // 1 done
	int8_t at132s_lock_config(SPI_HandleTypeDef *hspi);
	int8_t at132s_get_lock_status(SPI_HandleTypeDef *hspi);
	int8_t at132s_get_serial(SPI_HandleTypeDef *hspi, uint8_t *data);
	int8_t at132s_get_random(SPI_HandleTypeDef *hspi, uint8_t *data);

	// keystorage
	void at132s_data_store(SPI_HandleTypeDef *hspi, uint16_t memaddy, uint8_t *data, uint8_t len);
	void at132s_data_load(SPI_HandleTypeDef *hspi, uint16_t memaddy, uint8_t *data, uint8_t len);
public:
	ATAES132_SPI();
	int8_t init( SPI_HandleTypeDef *hspi, osMutexId_t in);
	int8_t config_device(SPI_HandleTypeDef *hspi);
	// thread
	uint8_t getRunningState();
	void startRunning();
	// rng
	void rtos_rng_update();
	// keystore
	void rtos_keystore_update();
};


#endif /* INC_ATAES132_SPI_HPP_ */
