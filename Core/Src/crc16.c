/*
 * crc16.c
 *
 *  Created on: 3 Sep 2022
 *      Author: Tom
 */

#include "stdint.h"



/** \This function calculates a 16‐bit CRC.
* \param[in] count number of bytes in data buffer
* \param[in] data pointer to data
* \param[out] crc pointer to calculated CRC (high byte at crc[0])
*
* source: ATAES132 datashgeet, p151
*/
void CalculateCrc(uint8_t length, uint8_t *data, uint8_t *crc)
{
	uint8_t counter;
	uint8_t crcLow = 0, crcHigh = 0, crcCarry;
	uint8_t polyLow = 0x05, polyHigh = 0x80;
	uint8_t shiftRegister;
	uint8_t dataBit, crcBit;
		for (counter = 0; counter < length; counter++) {
			for (shiftRegister = 0x80; shiftRegister > 0x00; shiftRegister >>= 1) {
				dataBit = (data[counter] & shiftRegister) ? 1 : 0;
				crcBit = crcHigh >> 7;
				// Shift CRC to the left by 1.
				crcCarry = crcLow >> 7;
				crcLow <<= 1;
				crcHigh <<= 1;
				crcHigh |= crcCarry;
				if ((dataBit ^ crcBit) != 0) {
					crcLow ^= polyLow;
					crcHigh ^= polyHigh;
					}
			}
	}
	crc[0] = crcHigh;
}
