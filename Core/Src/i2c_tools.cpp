/*
 * i2c_tools.c
 *
 *  Created on: Aug 19, 2022
 *      Author: Tom
 *
 */


#include "i2c_tools.hpp"
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

#include "stm32h7xx_hal_i2c.h"

I2C_HandleTypeDef *i2c1h;
I2C_HandleTypeDef *i2c2h;

/**
 * Initialise local I2C so cleaner functions can be used
 */
int8_t i2c1_init(I2C_HandleTypeDef *I2cHandle)
{
	*i2c1h = *I2cHandle;
	return 0;
}

/**
 * Send a single value to a reg on a device
 * @param devaddy device address. 7 bit, 8th is a r/w flag. xxxxxxx[R/W]
 * @param register address
 * @param value to set
 * usage:
 * i2c1_init(&hi2c1);
 * i2c_send(devaddy, regaddy, value);
 */
int8_t i2c1_send( uint16_t devaddy, uint16_t regaddy, uint8_t value)
{
	// fix ST's I2C bug
	devaddy = (devaddy << 1);

#ifdef I2C1_POLLING
	if(HAL_I2C_Mem_Write(i2c1h, devaddy, regaddy, (uint16_t) 0x01, &value, 1, 10000)!= HAL_OK)
	{
		#ifdef SWD_DEBUG
		uint8_t debugstr[12] = "I2C1 TX ERR ";
		SWD_Write(&debugstr, (uint32_t)sizeof(debugstr));
		#endif
		return -1;
	}
#endif

	return 0;
}

/**
 * Send a block of data to I2C device
 */
int8_t i2c1_send_block( uint16_t devaddy, uint16_t regaddy, uint8_t *value, uint16_t len)
{

	devaddy = (devaddy << 1);

#ifdef I2C1_POLLING
	if(HAL_I2C_Mem_Write(i2c1h, devaddy, regaddy, (uint16_t) 0x01, value, len, 10000)!= HAL_OK)
	{
		#ifdef SWD_DEBUG
		uint8_t debugstr[12] = "I2C1 TX ERR ";
		SWD_Write(&debugstr, (uint32_t)sizeof(debugstr));
		#endif
		return -1;
	}
#endif

	return 0;
}


/**
 * Returns the value by reference (ptr). value indicates success or failure
 */
int8_t i2c1_read( uint16_t devaddy, uint16_t regaddy, uint8_t *value)
{
	#define RXBUFFERSIZE 1
	devaddy = (devaddy << 1);

		#ifdef I2C1_POLLING
		if(HAL_I2C_Mem_Read(i2c1h, devaddy, regaddy, 0x01, value, RXBUFFERSIZE, 1000)!= HAL_OK)
		{
			#ifdef SWD_DEBUG
			const uint8_t debugstr[12] = "I2C1 RX ERR ";
			SWD_Write(&debugstr, (uint32_t)sizeof(debugstr));
			#endif
			return -1;
		}
		#endif

		#ifdef I2C1_DMA
		#endif

		return 0;
}

int8_t i2c1_read_block( uint16_t devaddy, uint16_t regaddy, uint8_t *value, uint16_t len)
{
	devaddy = (devaddy << 1);

		#ifdef I2C1_POLLING
		if(HAL_I2C_Mem_Read(i2c1h, devaddy, regaddy, 0x01, value, len, 1000)!= HAL_OK)
		{
			#ifdef SWD_DEBUG
			const uint8_t debugstr[12] = "I2C1 RX ERR ";
			SWD_Write(&debugstr, (uint32_t)sizeof(debugstr));
			#endif
			return -1;
		}
		#endif

		#ifdef I2C1_DMA
		#endif

		return 0;
}


/********************************************************************************************************************
 * V2
 */



int8_t i2c_set_reg()
{

}

int8_t i2c_write( uint16_t devaddy, uint8_t *data, uint8_t len)
{
	// fix ST's I2C bug
	//devaddy = (devaddy << 1);

	//if(HAL_I2C_Mem_Write(hi2c, devaddy, regaddy, (uint16_t) 0x01, value, len, 0)!= HAL_OK)
#ifdef I2C1_POLLING
	if(HAL_I2C_Master_Transmit(i2c2h, devaddy, data, len, 10) != HAL_OK)
	{
		#ifdef SWD_DEBUG
		uint8_t debugstr[12] = "I2C2 TX ERR ";
		SWD_Write(&debugstr, (uint32_t)sizeof(debugstr));
		#endif
		return -1;
	}
#endif


	return 0;
}

int8_t i2c_read_write(uint16_t devaddy, uint8_t *txbuff, uint16_t txlen, uint8_t *rxbuff, uint16_t rxlen)
{
	uint16_t timeout = 0;
	//devaddy = (devaddy << 1);
	//uint8_t regaddy = *txbuff++;

	//HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
	//if(HAL_I2C_Mem_Write(hi2c, devaddy, regaddy, (uint16_t) 0x01, value, len, 0)!= HAL_OK)
	if( HAL_I2C_Master_Transmit(i2c2h, (uint16_t)devaddy, (uint8_t*) txbuff, (int16_t) txlen, (uint32_t) 500) != 0x00 )
	{
		/* Error_Handler() function is called when error occurs. */
		//Error_Handler();
		//return -1;
	}
	HAL_Delay(60);

	if( HAL_I2C_Master_Receive(i2c2h, (uint16_t) devaddy, (uint8_t*) rxbuff, (int16_t) rxlen, (uint32_t) 500) != 0x00 )
	{
		/* Error_Handler() function is called when error occurs. */
		//Error_Handler();
		//return -3;
	}
	return 0;
}

int8_t i2c_read_write_2( uint16_t devaddy, uint16_t regaddy, uint8_t *txbuff, uint16_t txlen, uint8_t *rxbuff, uint16_t rxlen)
{
	uint16_t timeout = 0;
	//devaddy = (devaddy << 1);

	//HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
	if(HAL_I2C_Mem_Write(i2c2h, devaddy, regaddy, (uint16_t) 0x02, txbuff, txlen, 1)!= HAL_OK)
	//if( HAL_I2C_Master_Transmit(i2c2h, (uint16_t)devaddy, txbuff, (int16_t) txlen, (uint32_t) 100) != 0x00 )
	{
		/* Error_Handler() function is called when error occurs. */
		//Error_Handler();
		//return -1;
	}

	//if(HAL_I2C_Mem_Read(i2c2h, devaddy, regaddy, 0x02, rxbuff, rxlen, 100)!= HAL_OK)
	//if( HAL_I2C_Master_Receive(i2c2h, (uint16_t) devaddy, (uint8_t*) rxbuff, (int16_t) rxlen, (uint32_t) 1) != HAL_OK )
	{
		/* Error_Handler() function is called when error occurs. */
		//Error_Handler();
		//return -3;
	}
	return 0;
}



