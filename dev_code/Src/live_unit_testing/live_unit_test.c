/*
 * live_unit_test.c
 *
 *  Created on: Aug 25, 2022
 *      Author: Tom
 */
// app
#include <internal_storage.h>
#include "_core_system_config.h"
#include "rng.h"
#include "GUI_State.h"
#include "ascii_keyboard.h"
#include "stdint.h"
#include "stdlib.h"
#include "cmsis_os2.h"


#ifdef LIVE_UNIT_TESTING

RNG_HandleTypeDef *test_hrng;
I2C_HandleTypeDef *test_hi2c1;
I2C_HandleTypeDef *test_hi2c2;
osMutexId_t *test_mutexGUI;
osMutexId_t *test_mutexDataBus;

/**
 * Create some setters to avoid having to completely remake the main thread
 */
void unit_test_setup_setRNG(RNG_HandleTypeDef *test_hrng_in)
{
	test_hrng = test_hrng_in;
}

void unit_test_setup_setI2C1(I2C_HandleTypeDef *test_hi2c1_in)
{
	test_hi2c1 = test_hi2c1_in;
}

void unit_test_setup_setI2C2(I2C_HandleTypeDef *test_hi2c2_in)
{
	test_hi2c2 = test_hi2c2_in;
}

void unit_test_setup_setGUIThread(osMutexId_t *test_mutexGUI_in)
{
	test_mutexGUI = test_mutexGUI_in;
}

void unit_test_setup_setDataBusThread(osMutexId_t *test_mutexDataBus_In)
{
	 test_mutexDataBus = test_mutexDataBus_In;
}

// functional tests
// edge case e.g buffer overflows, bad inputs if possible by user

void testSetUp()
{
	// need a basic init function in main to get:
	// - threads
	// - global state
}

void testTearDown()
{

}



/**************************************************************************************************************************************
 * Display tests
 */

int8_t test_display_showAllScreens()
{
	_gui_startup(0);
	HAL_delay(1000);
	
	_gui_standby(0);
	HAL_delay(1000);
	
	for (uint i=0; i<4; i++)
	{
		_gui_send_password_n(0, i);
		HAL_delay(500);
	}
	
	_gui_hold_for_erase(0);
	HAL_delay(1000);
	
	for (uint i=1; i<7; i++)
	{
		_gui_erasing(i);
		HAL_delay(500);
	}
	
	for (uint i=1; i<7; i++)
	{
		_gui_generating_keys(i);
		HAL_delay(500);
	}
}

void test_display_loopBehaviour()
{
	// set OS state
	osMutexId_t mutexGUIHandle;
	
	global_gui_state = GUI_STANDBY;
	os_gui_loop(mutexGUIHandle);
	HAL_delay(1000);
	
	global_gui_state = GUI_HOLD_FOR_ERASE;
	os_gui_loop(mutexGUIHandle);
	HAL_delay(1000);
	
	global_gui_state = GUI_ERASING;
	os_gui_loop(mutexGUIHandle);
	HAL_delay(1000);
	
	global_gui_state = GUI_ERASING_DONE;
	os_gui_loop(mutexGUIHandle);
	HAL_delay(1000);
	
	global_gui_state = GUI_GENERATING_KEYS;
	os_gui_loop(mutexGUIHandle);
	HAL_delay(1000);
	
	global_gui_state = GUI_GENERATING_KEYS_DONE;
	os_gui_loop(mutexGUIHandle);
	HAL_delay(1000);
	
	global_gui_state = GUI_SEND_PASSWORD_1;
	os_gui_loop(mutexGUIHandle);
	HAL_delay(1000);
	
	global_gui_state = GUI_SEND_PASSWORD_DONE;
	os_gui_loop(mutexGUIHandle);
	HAL_delay(1000);
	
	global_gui_state = GUI_SLEEP;
	os_gui_loop(mutexGUIHandle);
	HAL_delay(1000);
	
	global_gui_state = GUI_SYSTEM_FAULT;
	os_gui_loop(mutexGUIHandle);
	HAL_delay(1000);
	
	// call display 
	//
	
}

/**************************************************************************************************************************************
 *  USB Keyboard tests
 */

// check conversion gives dcorrect results: pass/fail
//uint8_t char_to_keyboard(char x)
int8_t test_keyboard_characterFormatConversion()
{
	int8_t pass = 0;

	char A2Zup[26] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
	char A2Zlo[26] = {"abcdefghijklmnopqrstuvwxyz"};
	char OneToNine[9] = {"123456789"};
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

	char A2Zup[26] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
	char A2Zlo[26] = {"abcdefghijklmnopqrstuvwxyz"};

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
	char A2Zup[26] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
	char A2Zlo[26] = {"abcdefghijklmnopqrstuvwxyz"};
	char ZeroToNine[10] = {"0123456789"};
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

/**************************************************************************************************************************************
 * Crypto storage tests
 */

int8_t test_crypto_storage_saveRecall()
{
	uint16_t key[16] = {'1','2','3','4','5','6','7','8','a','b','c','d','e','f','g','h'};
	uint16_t readkey[16];
	uint16_t readkey2[16];
	int8_t pass = 0;

	storage_init();

	// save, load, compare
	store_pwd1(&key[0]);
	load_pwd1(&readkey[0]);
	for (uint8_t i=0; i<16; i++)
	{
		if (key[i] != readkey[i])
		{
			// err
			pass = -1;
		}
	}

	// save, load, compare
	store_pwd2(&key[0]);
	load_pwd2(&readkey2[0]);
	for (uint8_t i=0; i<16; i++)
	{
		if (key[i] != readkey2[i])
		{
			// err
			pass = -2;
		}
	}
	return pass;
}

int8_t test_crypto_storage_erase()
{
	uint16_t key[16] = {'1','2','3','4','5','6','7','8','a','b','c','d','e','f','g','h'};
	uint16_t readkey[16];
	uint16_t readkey2[16];
	int8_t pass = 0;

	storage_init();

	// erase
	erase_block_default();
	load_pwd1(&readkey[0]);
	for (uint8_t i=0; i<16; i++)
	{
		if (0x00 != readkey[i])
		{
			// err
			pass = -1;
		}
	}

	// verify
	load_pwd1(&readkey2[0]);
	for (uint8_t i=0; i<16; i++)
	{
		if (0x00 != readkey2[i])
		{
			// err
			pass = -1;
		}
	}
	return pass;
}

/**************************************************************************************************************************************
 * keypad tests
 */

// requires gui thread
// state should be 50+i for the first 9
// not finished. needs to consider button press in state context
void test_keypad_readFakeInputsSingle(void)
{
	// set fake values
	uint8_t len = 9;
	uint8_t fakevals[12] = {0,0,0,0, 0,0,0,0, 0,0,0,0};
	osMutexId_t mutexGUIHandle;

	#ifdef KEY_COUNT_12
	len = 12;
	#endif

	// check single key presses
	for (uint8_t i=0; i<len; i++)
	{
		fakevals[i] = 1;
		keypad_set_fake_values(&fakevals, len);

		

		// run test
		os_keypad_loop(&mutexGUIHandle);
		
		// reset
		fakevals[i] = 0;
	}
}

// state should be unchanged
void test_keypad_readFakeInputsSingleWhenErasing(void)
{
}

void test_keypad_readFakeInputsMultipleKeys(void)
{
	// GUI_State process_keypad(GUI_State ui_state, uint8_t *button_values)
}

/*
void os_keypad_loop(osMutexId *mutexIn);

// private functions
void read_keypad(uint8_t *button_values);
GUI_State process_single_press(GUI_State ui_state, uint8_t key);
GUI_State process_keypad(GUI_State ui_state, uint8_t *button_values);
*/

/**************************************************************************************************************************************
 * Logic loop tests
 */
 
// test that logic loop takes and returns global state
// GUI_SYSTEM_FAULT ->  GUI_STARUP

// 456

/**************************************************************************************************************************************
 * Internal RNG tests
 */

void test_internal_rng_generateDifferentNumbers()
{
	uint8_t arr1[16];
	uint8_t arr2[16];
	int8_t pass = 0;

	// check that 8 characters are returned 
	generate_random_string_8(test_hrng, &arr1[0]);
	generate_random_string_8(test_hrng, &arr2[0]);
	// make sure they arent the same
	uint32_t sum1, sum2;
	for (uint8_t i=0; i<8; i++)
	{
		sum1 = sum1 + arr1[i];
		sum2 = sum2 + arr2[i];
	}
	if(sum1 == sum2){pass = -1;}

	// zero the array and sum1
	for (uint8_t i=0; i<16; i++)
	{
		arr1[i] = 0;
		arr2[i] = 0;
	}
	sum1 = 0; sum2 = 0;
	
	// check that 16 characters are returned
	generate_random_string_16(test_hrng, &arr1[0]);
	generate_random_string_16(test_hrng, &arr2[0]);
	// make sure they arent the same
	for (uint8_t i=0; i<16; i++)
	{
		sum1 = sum1 + arr1[i];
		sum2 = sum2 + arr2[i];
	}
	if(sum1 == sum2){pass = -1;}
}

/**************************************************************************************************************************************
 * Multithreading tests
 */

// GUI mutex test

// data bus mutex test

/**************************************************************************************************************************************
 * I2C secure module tests
 tbf
 */
 
 // init device
 // read back and verify register settings
 
 // getrandom value unlocked. verify it is the expected test value
 
 // generate two lots of numbers and make sure they arent the same



/**************************************************************************************************************************************
 * Sleep mode tests
 */

/**************************************************************************************************************************************
 * Flash storage tests
 */

/**************************************************************************************************************************************
 * Security assurance tests
 */

void live_unit_test_main()
{
	// if SWD print isn't enabled then enable it
	
	// setUp function
}

#endif
