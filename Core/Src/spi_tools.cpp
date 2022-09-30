/*
 * spi_tools.cpp
 *
 *  Created on: Sep 14, 2022
 *      Author: Tom
 */

#include "stdint.h"
#include "spi_tools.hpp"
#include "cmsis_os2.h"

osMutexId_t spimutex;

void spi_init(osMutexId_t in) {
	spimutex = in;
}

int8_t spi_send(SPI_HandleTypeDef *hspi, uint8_t *tx, uint8_t *rx, uint16_t len) {
	if(spimutex != NULL) {
		int32_t OsStatus = 0;
		OsStatus = osMutexAcquire(spimutex, 100);
		if (OsStatus == osOK) {
			#ifdef SPI1_USE_DMA
			//HAL_StatusTypeDef HAL_SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
			HAL_SPI_TransmitReceive_DMA(hspi, tx, rx, (uint16_t) len);
			#else
			HAL_SPI_TransmitReceive(hspi, tx, rx, len, 1000 );
			#endif
			OsStatus = osMutexRelease(spimutex);
		}
		osDelay(1);
	}
	return 0;
}
