/*
 * spi_tools.hpp
 *
 *  Created on: Sep 14, 2022
 *      Author: Tom
 */

#ifndef INC_SPI_TOOLS_HPP_
#define INC_SPI_TOOLS_HPP_



#include "stdint.h"
#include "stm32h7xx_hal.h"
#include "cmsis_os2.h"

void spi_init(osMutexId_t in);
int8_t spi_send(SPI_HandleTypeDef *hspi, uint8_t *tx, uint8_t *rx, uint16_t len);


#endif /* INC_SPI_TOOLS_HPP_ */
