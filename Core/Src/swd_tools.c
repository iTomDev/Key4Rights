/*
 * swd_tools.c
 *
 *  Created on: Jul 22, 2022
 *      Author: Tom
 */

// app
#include "delay_tools.h"
#include "swd_tools.h"
// api
#include "stdlib.h"


// works
//uint8_t test[4] = "abcd";
//SWD_Write(&test, 4);
void SWD_Write(uint8_t *ptr, uint32_t len)
{
    for (int i = 0; i < len; i++)
    {
        ITM_SendChar(*ptr++);
        delayms(10);
    }
}

void SWD_Write_Char16(uint16_t val)
{
	//for (int i = 0; i < len; i++)
	{
		uint16_t num = 'e';
		num = (char) val;
		ITM_SendChar(num);
		delayms(10);
	}
}

void SWD_Write_Char16_Array(uint16_t *val, uint16_t len)
{
	for (int i = 0; i < len; i++)
	{
		uint16_t num = 'e';
		num = (char) val[i];
		ITM_SendChar(num);
		delayms(10);
	}
}

void SWD_Write_Uint8(uint8_t num, uint32_t len)
{
	char chars[3]; // max 256
	uint8_t charslen = 0;
	utoa(num, &chars[0], 10);
	charslen = 1;
	if (num>9)
	{
		charslen = 2;
		if (num>99)
		{
			charslen = 3;
		}
	}

    for (int i = 0; i < charslen; i++)
    {

        ITM_SendChar(chars[i]);
        delayms(10);;
    }
}

//uint8_t test[4] = {0, 55, 128, 255};
//SWD_Write_Uint8_Array(&test, 4);
//HAL_Delay(200);
void SWD_Write_Uint8_Array(uint8_t *ptr, uint32_t len)
{

	// loop through the array of numbers
	for(int i=0; i<len; i++)
	{
		// convert one to a string
		char chars[3]; // max 256
		uint8_t charslen = 0;
		unsigned int num = 0;
		num = *ptr;
		ptr++;
		utoa(num, &chars[0], 10);

		charslen = 1;
		if (num>9)
		{
			charslen = 2;
			if (num>99)
			{
				charslen = 3;
			}
		}
		// copy to return array
		for (uint8_t j=0; j<charslen; j++)
		{
			ITM_SendChar(chars[j]);
			delayms(10);
		}
		ITM_SendChar(',');
		delayms(10);
		ITM_SendChar(' ');
		delayms(10);
	}
	ITM_SendChar('\n');
	delayms(10);
}


void SWD_Write_Uint16_Array(uint8_t *ptr, uint32_t len)
{
	// loop through the array of numbers
	for(int i=0; i<len; i++)
	{
		// convert one to a string
		char chars[5]; // max 65535
		uint8_t charslen = 0;
		unsigned int num = 0;
		num = *ptr;
		ptr++;
		utoa(num, &chars[0], 10);

		charslen = 1;
		if (num>9)
		{
			charslen = 2;
			if (num>99)
			{
				charslen = 3;
				if (num>999)
				{
					charslen = 4;
					if (num>9999)
					{
						charslen = 5;
					}
				}
			}
		}
		// copy to return array
		for (uint8_t j=0; j<charslen; j++)
		{
			ITM_SendChar(chars[j]);
			delayms(10);
		}
		ITM_SendChar(',');
		delayms(10);
		ITM_SendChar(' ');
		delayms(10);
	}
	ITM_SendChar('\n');
	delayms(10);
}
