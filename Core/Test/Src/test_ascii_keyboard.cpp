/*
 * test_ascii_keyboard.cpp
 *
 *  Created on: Sep 22, 2022
 *      Author: Tom
 */


#include "stdint.h"
#include "ascii_keyboard.hpp"


/**************************************************************************************************************************************
 *  USB Keyboard tests
 */

// check conversion gives dcorrect results: pass/fail
//uint8_t char_to_keyboard(char x)
int8_t test_keyboard_characterFormatConversion()
{
	int8_t pass = 0;

	char A2Zup[27] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
	char A2Zlo[27] = {"abcdefghijklmnopqrstuvwxyz"};
	char OneToNine[10] = {"123456789"};
	uint8_t res;

	// upper case
	for(uint8_t i=0; i<26; i++)
	{
		res = char_to_keyboard(A2Zup[i]);
		if (res != (0x04 + i))
		{
			pass = -1;
		}
	}

	// lower case
	for(uint8_t i=0; i<26; i++)
	{
		res = char_to_keyboard(A2Zlo[i]);
		if (res != (0x04 + i))
		{
			pass = -2;
		}
	}

	// numbers 1-9
	for(uint8_t i=0; i<9; i++)
	{
		res = char_to_keyboard(OneToNine[i]);
		if (res != (0x1D + i))
		{
			pass = -3;
		}
	}

	// number 0
	res = char_to_keyboard((char)'0');
	if (res != 0x26)
	{
		pass = -4;
	}

	return pass;
}

// check that the correct case is returned: pass/fail
//uint8_t char_to_keyboard_case(char x)
int8_t test_keyboard_caseCategorisation()
{
	int8_t pass = 0;

	char A2Zup[27] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
	char A2Zlo[27] = {"abcdefghijklmnopqrstuvwxyz"};

	// upper case
	for(uint8_t i=0; i<26; i++)
	{
		uint8_t res = char_to_keyboard_case(A2Zup[i]);
		if (res != 0x04)
		{
			pass = -1;
		}
	}

	// lower case
	for(uint8_t i=0; i<26; i++)
	{
		uint8_t res = char_to_keyboard_case(A2Zlo[i]);
		if (res != 0x00)
		{
			pass = -2;
		}
	}

	return pass;
}

// check values appear on screen in notepad or the like - user verified
// uses shift key modifier so press the real shift key a couple times after stopping the test!
void test_keyboard_sendSequenceToUSB()
{
	char A2Zup[27] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
	char A2Zlo[27] = {"abcdefghijklmnopqrstuvwxyz"};
	char ZeroToNine[11] = {"0123456789"};
	uint8_t res;

	// upper case
	for(uint8_t i=0; i<26; i++)
	{
		res = char_to_keyboard(A2Zup[i]);
		HAL_Delay(20);
	}

	// lower case
	for(uint8_t i=0; i<26; i++)
	{
		res = char_to_keyboard(A2Zlo[i]);
		HAL_Delay(20);
	}

	// numbers 0-9
	for(uint8_t i=0; i<10; i++)
	{
		res = char_to_keyboard(ZeroToNine[i]);
		HAL_Delay(20);
	}
}
