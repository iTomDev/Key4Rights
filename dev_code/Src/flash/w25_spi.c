/*
 * w25_spi.c
 *
 *  Created on: Aug 29, 2022
 *      Author: Tom
 *
 *      W25 driver. For use with FATFS.
 */

#include "stdint.h"


// commands
#define W25_SPI_CMD_SECTOR_ERASE_4KB	0x20
#define W25_SPI_CMD_BLOCK_ERASE_32KB	0x52
#define W25_SPI_CMD_BLOCK_ERASE_64KB	0xD8
#define W25_SPI_CMD_READ				0x03
#define W25_SPI_CMD_WRITE				0x02

#define W25_SPI_PARAM_DUMMY



/**
 * Primary entry functions
 */

void w25_spi_init()
{

}

void w25_spi_write()
{

}

void w25_spi_read()
{

}
