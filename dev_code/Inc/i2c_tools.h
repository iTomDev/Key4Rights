/*
 * i2c_tools.h
 *
 *  Created on: Aug 19, 2022
 *      Author: Tom
 */

#ifndef INC_I2C_TOOLS_H_
#define INC_I2C_TOOLS_H_

#include "_core_system_config.h"
#ifdef STM32H7
#include "stm32h7xx_hal.h"
#endif
#ifdef STM32F103
#include "stm32f1xx_hal.h"
#endif
#ifdef STM32F217
#include "stm32f2xx_hal.h"
#endif

#define I2C1_POLLING
#define I2C2_POLLING
#define I2C3_POLLING
//#define I2C1_DMA
//#define I2C2_DMA
//#define I2C3_DMA

int8_t i2c1_init(I2C_HandleTypeDef *I2cHandle);

int8_t i2c1_send( uint16_t devaddy, uint16_t regaddy, uint8_t value);

int8_t i2c1_send_block( uint16_t devaddy, uint16_t regaddy, uint8_t *value, uint16_t len);

int8_t i2c1_send_setup_block(uint16_t devaddy, uint16_t *regaddy, uint8_t *value, uint8_t len);

int8_t i2c1_write_then_read(uint16_t devaddy, uint8_t *txbuff, uint16_t txlen, uint8_t *rxbuff, uint16_t rxlen);

#endif /* INC_I2C_TOOLS_H_ */
