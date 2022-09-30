/*
 * keypad_ui.hpp
 *
 *  Created on: Sep 22, 2022
 *      Author: Tom
 */

#ifndef INC_KEYPAD_UI_HPP_
#define INC_KEYPAD_UI_HPP_

// lib
#include "cmsis_os.h"
// app
#include "_core_system_config.h"
#include "swd_tools.h"
#include "gui_state.hpp"

// system can operate as a key device, storage device SD, storage text/flash, or in a config mode
#define SYSTEM_MODE_KEYGEN					0
#define SYSTEM_MODE_CONFIG					1
#define SYSTEM_MODE_DRIVE					2

// constants for processing keypad correctly e./g debouncing, double press detection
#define BUTTON_ON_TRESHOLD					10		// debounce threshold
#define BUTTON_REPRESS_DELAY				10
#define BUTTON_HELD_ON_TRESHOLD				100		//
#define BUTTON_TRANSIENT_TIMEOUT_TRESHOLD	10	// how many cycles before resetting the button count due to inactivity. stops transients building up
//#define KEY_COUNT_12

// OS
#define OS_KEYBOARD_UPDATE_PERIOD			50 // 50ms = 20Hz

// test functions
#ifdef LIVE_UNIT_TESTING
void keypad_set_fake_values(uint8_t *arr, uint8_t len);
#endif

//public functions
void os_keypad_loop();

// private functions
void read_keypad(uint8_t *button_values);
GUI_State process_single_press(GUI_State ui_state, uint8_t key);
GUI_State process_keypad(GUI_State ui_state, uint8_t *button_values);



#endif /* INC_KEYPAD_UI_HPP_ */
