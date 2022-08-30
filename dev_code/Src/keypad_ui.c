/*
 * keypad_ui.c
 *
 *  Created on: Aug 17, 2022
 *      Author: Tom
 */

// keypad config - LQFP100: STM32F217
// In 1,2,3 = D3,D4,D5
// Out 1,2,3 = D6,D7,B3

// keypad config - LQFP48: STM32F103
// In 1,2,3 = A8,B2,B1
// Out 1,2,3 = B0,B4,B5

//  RTOS
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
// app
#include "keypad_ui.h"
#include "GUI_State.h"



/**
 * Helper functions
 */
#ifdef LIVE_UNIT_TESTING
uint8_t keypad_fake_values[12];

void keypad_set_fake_values(uint8_t *arr, uint8_t len)
{
	for(uint8_t i=0; i<len; i++)
	{
		keypad_fake_values[i] = *arr++;
	}
}
#endif

/**
 * Processes user input from a keypad and gives simple UI output from LEDs
 * Keypad: 3x3 or 3x4 scanned
 */
void read_keypad(uint8_t *button_values)
{
	//uint8_t button_temp[9] = {0,0,0,0,0,0,0,0,0,0,0,0};

	// scan through output drive ports
	for (uint8_t i=0; i<3; i++)
	{
#ifdef STM32F103
		// switch output i on and the others off
		if (i==0)  // 0
		{
			// set one high, others low
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
		}
		else
		{
			if (i>1) // 2
			{
				// set one high, others low
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
			}
			else //1
			{
				// set one high, others low
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
			}
		}
		// test the input level across the inputs
		button_values[(i*3)+0] = (uint8_t) HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
		button_values[(i*3)+1] = (uint8_t) HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
		button_values[(i*3)+2] = (uint8_t) HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
		#ifdef KEY_COUNT_12
			button_values[(i*3)+3] = (uint8_t) HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
		#endif
#endif

#ifdef STM32F217
		// switch output i on and the others off
		if (i==0)  // 0
		{
			// set one high, others low
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
		}
		else
		{
			if (i>1) // 2
			{
				// set one high, others low
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 0);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
			}
			else //1
			{
				// set one high, others low
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 0);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
			}
		}
		// test the input level across the inputs
		button_values[(i*3)+0] = (uint8_t) HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3);
		button_values[(i*3)+1] = (uint8_t) HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4);
		button_values[(i*3)+2] = (uint8_t) HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5);
#endif

#ifdef LIVE_UNIT_TESTING
		// inject fake values from helper function
		#ifdef KEY_COUNT_12

		for (uint8_t i=0; i<12; i++)
		{
			*button_values++ = keypad_fake_values[i];
		}
		#else // 9
		for (uint8_t i=0; i<9; i++)
		{
			*button_values++ = keypad_fake_values[i];
		}
		#endif
#endif
	}
}

/**
 * Process the case when a single button is pressed. most common.
 */
GUI_State process_single_press(GUI_State ui_state, uint8_t key)
{
	// can be expanded easily if required

	// if generating keys or erasing effectively block the UI
	if (ui_state == GUI_ERASING || ui_state == GUI_GENERATING_KEYS || ui_state == GUI_ERASING_DONE || ui_state == GUI_GENERATING_KEYS_DONE)
	{}
	else
	{
		return 50 + key;
	}
	return ui_state;
}


/**
 * Process keypad, mainly to deal with two keys at the same time and to debounce keys
 */
GUI_State process_keypad(GUI_State ui_state, uint8_t *button_values)
{
	// local copy of state
	//GUI_State ui_state = 0;

	// read button states
	// non interrupt based GPIO test
	//uint8_t system_state = 0;
	uint8_t system_mode = 0;
	uint8_t button_temp[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	uint32_t button_counter[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	uint8_t button_state[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	uint32_t button_transient_timeout_count = 0;
	uint8_t reset_threshold_exceeded = 0;

	// loop through buttons
	for (uint8_t i=0; i<12; i++)
	{
		// if button is on
		if (button_temp[i] > 0 && button_state[i] == 0)
		{
			// increase count
			button_counter[i]++;

			// if above threshold
			if(button_counter[i] > BUTTON_ON_TRESHOLD)
			{
				button_state[i] = BUTTON_REPRESS_DELAY;
				// button held down threshold
				if(button_counter[i] > BUTTON_HELD_ON_TRESHOLD)
				{
					button_state[i] = BUTTON_REPRESS_DELAY;
					reset_threshold_exceeded = 1;
				}
			}
		}
		else
		{
			// button handled, wait period until the button can be pushed again. Trailing edge debounce effectively
			if(button_state[i] > 0)
			{
				// decrement count, prevents double counting
				button_state[i]--;
			}
		}
	}//for

	// are multiple buttons being held down (i.e more than 1)
	if (system_mode == SYSTEM_MODE_KEYGEN)
	{
		// if multiple buttons pressed
		uint32_t button_sum = button_state[0] + button_state[1] + button_state[2] + button_state[3] + button_state[4] + button_state[5] + button_state[6]  + button_state[7]  + button_state[8];
		if(button_sum > (BUTTON_ON_TRESHOLD * 2) )
		{
			// how long for?
			ui_state = GUI_HOLD_FOR_ERASE;

			// if a button has exceeded the held down threshold in multibutton detection mode then reset
			if(reset_threshold_exceeded > 0)
			{
				ui_state = GUI_ERASING;
			}
		}
		else	// single button
		{
			// process any single button presses and then reset the state
			for (uint8_t i=0; i<9; i++)
			{
				if (button_state[i] > BUTTON_ON_TRESHOLD)
				{
					button_state[i] = BUTTON_REPRESS_DELAY;
				}
				// call dedicates processor
				process_single_press(ui_state, i);
			}
		}
	}

	// prevent accumulation of transients summing to a button press or worse still a multibutton reset!
	button_transient_timeout_count++;
	if (button_transient_timeout_count > BUTTON_TRANSIENT_TIMEOUT_TRESHOLD)
	{
		// loop through and rezero all counts
		for (uint8_t i=0; i<12; i++)
		{
			button_counter[i] = 0;
		}
	}
	return ui_state;
}

/**
 * Main entry points
 */

/**
 * Operating system keypad loop
 */
void os_keypad_loop(osMutexId *mutexIn)
{
	GUI_State ui_state;
	osMutexId mutex;
	mutex = &mutexIn;

	uint8_t button_temp[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

	while(1)
	{
		// get latest state
		if (mutex != NULL)
		{
			int32_t OsStatus = 0;
			OsStatus = osMutexWait(mutex, 100);
			if (OsStatus == osOK)
			{
				// critical data exchange only
				ui_state = global_gui_state;
				OsStatus = osMutexRelease(mutex);
			}
		}

		// read button states
		read_keypad(&button_temp[0]);
		// process buttons
		ui_state = process_keypad(ui_state, &button_temp[0]);

		// return update to global state
		if (mutex != NULL)
		{
			int32_t OsStatus = 0;
			OsStatus = osMutexWait(mutex, 100);
			if (OsStatus == osOK)
			{
				// critical data exchange only
				global_gui_state = ui_state;
				OsStatus = osMutexRelease(mutex);
			}
		}

		osDelay(OS_KEYBOARD_UPDATE_PERIOD);
	}
}
