/*
 * test_keypad_ui.cpp
 *
 *  Created on: Sep 22, 2022
 *      Author: Tom
 */

#include "stdint.h"
#include "cmsis_os2.h"
#include "../Inc/keypad_ui.hpp"


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
	uint8_t fakevals[13] = {0,0,0,0, 0,0,0,0, 0,0,0,0};
	osMutexId_t mutexGUIHandle;

	#ifdef KEY_COUNT_12
	len = 12;
	#endif

	// check single key presses
	for (uint8_t i=0; i<len; i++)
	{
		fakevals[i] = 1;
		keypad_set_fake_values(&fakevals[0], len);



		// run test
		os_keypad_loop();

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
