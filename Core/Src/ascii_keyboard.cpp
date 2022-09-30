/*
 * ascii_keyboard.c
 *
 *  Created on: Jul 20, 2022
 *      Author: Tom
 */

// API
#include "cmsis_os.h"
// App
#include "ascii_keyboard.hpp"
#include "_core_system_config.h"

// hex	key		ascii
// shift on i.e mod=2
// 04	A
// 1C	Y
// 1D	Z
// 1E	1
// 1F	2
// 24	7
// 25	8
// 26	9
// 27	0
// 28 [enter]
// 2D	-
// 2E	=
// 2F	[
// 30	]
// 31	#
// shift off i.e mod=0
// 04	a
// 05	b
// 06	c

extern USBD_HandleTypeDef hUsbDeviceFS;

/*
typedef struct
{
	uint8_t MODIFIER;
	uint8_t RESERVED;
	uint8_t KEYCODE1;
	uint8_t KEYCODE2;
	uint8_t KEYCODE3;
	uint8_t KEYCODE4;
	uint8_t KEYCODE5;
	uint8_t KEYCODE6;
}subKeyBoard;

subKeyBoard keyBoardHIDsub = {0,0,0,0,0,0,0,0};
*/

// breaks in c++ idk why, best fix is this
#define MODIFIER		0
#define RESERVED		1
#define KEYCODE1		2
#define KEYCODE2		3
#define KEYCODE3		4
#define KEYCODE4		5
#define KEYCODE5		6
#define KEYCODE6		7
uint8_t keyBoardHIDsub[8] = {0,0,0,0,0,0,0,0};


/********************************************************************************************************
 * Helper Functions
 ********************************************************************************************************
 */


/**
 * Returns what case the letter is
 * @return 0 lower, 4 upper. Because these are the values used in the USB protocol
 */
uint8_t char_to_keyboard_case(char x)
{
	x = (uint8_t) x;

	if( x > 0x40 && x < 0x5B)
	{
		return 0x02;
	}
	return 0;
}

/**
 * Converts ASCII character to USB Keyboard representation
 * @param character
 * @return Returns 0 if failed, otherwise the converted value
 * Limitation: Only supports A-Z, a-z, 0-9. It is possible to implement others but they aren't required for passwords in this implementation
 * as they aren't mapped into the RNG.
 *
 */
uint8_t char_to_keyboard(char x)
{
	x = (uint8_t) x;

	// upper case
	if( x > 0x40 && x < 0x5B)
	{
		x = x - 0x41;
		x = x + 0x04;
		return x;
	}
	// lower case
	if( x > 0x60 && x < 0x7B)
	{
		x = x - 0x61;
		x = x + 0x04;
		return x;
	}
	// 1 to 9
	if( x > 0x30 && x < 0x3A)
	{
		x = x - 0x31;
		x = x + 0x1E;
		return x;
	}
	// if 0: in HID 0 is at the end, ASCII it is at the start
	if (x == 0x30)
	{
		x = 0x27;
		return x;
	}
	return 0;
}


/********************************************************************************************************
 * Primary entry functions
 ********************************************************************************************************
 */

void init_ascii_keyboard()
{

}


/**
 * Send a character to the keyboard
 * Usage: send_char_to_keyboard('E');
 */
void send_char_to_keyboard(char x)
{
	uint8_t keycode = char_to_keyboard(x);
	uint8_t keycase = char_to_keyboard_case(x);

	//uint8_t USBD_HID_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report, uint16_t len);
#ifdef USING_OS
	// c++ not working fix
	//keyBoardHIDsub.MODIFIER=keycase;  // To press shift key<br>keyBoardHIDsub.KEYCODE1=0x04;  // Press A key
	//keyBoardHIDsub.KEYCODE1=keycode;  // Press letter key
	keyBoardHIDsub[MODIFIER]=keycase;
	keyBoardHIDsub[KEYCODE1]=keycode;
	USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub[0],sizeof(keyBoardHIDsub));
	osDelay(50);
	//keyBoardHIDsub.MODIFIER=0x00;  // To press shift key<br>keyBoardHIDsub.KEYCODE1=0x04;  // Press A key
	//keyBoardHIDsub.KEYCODE1=0x00;  // Press letter key
	keyBoardHIDsub[MODIFIER]=0x00;  // To press shift key<br>keyBoardHIDsub.KEYCODE1=0x04;  // Press A key
	keyBoardHIDsub[KEYCODE1]=0x00;  // Press letter key
	USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub[0],sizeof(keyBoardHIDsub));
	osDelay(50);
#else
	keyBoardHIDsub.MODIFIER=keycase;  // To press shift key<br>keyBoardHIDsub.KEYCODE1=0x04;  // Press A key
	keyBoardHIDsub.KEYCODE1=keycode;  // Press letter key
	USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub,sizeof(keyBoardHIDsub));
	HAL_Delay(50);
	keyBoardHIDsub.MODIFIER=0x00;  // To press shift key<br>keyBoardHIDsub.KEYCODE1=0x04;  // Press A key
	keyBoardHIDsub.KEYCODE1=0x00;  // Press letter key
	USBD_HID_SendReport(&hUsbDeviceFS,&keyBoardHIDsub,sizeof(keyBoardHIDsub));
	HAL_Delay(50);
#endif
}


/**
 * Sends an array of characters to the keyboard
 *
 * Usage:
 * uint8_t pwd[4] = "YxR5";
 * send_char_array_to_keyboard(&pwd, 4);
 */
void send_char_array_to_keyboard(uint8_t* arr, uint8_t len)
{
	// loop through array
	for (uint8_t i=0; i<len; i++)
	{
		send_char_to_keyboard(*arr);
		arr++;
	}
}

