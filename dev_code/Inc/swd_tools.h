/*
 * swd_tools.h
 *
 *  Created on: Jul 22, 2022
 *      Author: Tom
 */

#ifndef INC_SWD_TOOLS_H_
#define INC_SWD_TOOLS_H_

#include "stdint.h"
#include "main.h"

// usage:
//uint8_t debugstr[3] = "abc";
//SWD_Write(&debugstr, (uint32_t)sizeof(debugstr));
//HAL_Delay(500);
void SWD_Write(uint8_t *ptr, uint32_t len);

void SWD_Write_Uint8(uint8_t num, uint32_t len);


// usage
//uint16_t temp = 'A';
//temp = key1r[i];
//SWD_Write_Char16(temp);
void SWD_Write_Char16(uint16_t val);

//usage
//uint16_t key1r[8];
//SWD_Write_Char16_Array(key1r, 8);
void SWD_Write_Char16_Array(uint16_t *val, uint16_t len);

void SWD_Write_Uint8_Array(uint8_t *ptr, uint32_t len);

void SWD_Write_Uint16_Array(uint8_t *ptr, uint32_t len);

#endif /* INC_SWD_TOOLS_H_ */
