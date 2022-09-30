/*
 * logic_loop.c
 *
 *  Created on: Aug 24, 2022
 *      Author: Tom
 */
//

// App
//#include "internal_storage.h"
#include <main.hpp>
#include "gui_state.hpp"
#include "_core_system_config.h"
#include "delay_tools.h"
#include "ascii_keyboard.hpp"
#include "usb_device.h"
#include "rngdev.hpp"
#include "keystore.hpp"
// api
#include "string.h"
#include "cmsis_os.h"


RNGDEV *rnga;
RNGDEV *rngb;
Keystore *ks;

/**
 * Initialise main data and usb loop
 */
void logic_main_init(){
}

void logic_main_set_rng(RNGDEV *dev) {
	*rnga = *dev;
}

void logic_main_set_rng_alt(RNGDEV *dev) {
	*rngb = *dev;
}

void logic_main_set_keystore(Keystore *ksin) {
	*ks = *ksin;
}

void logic_main_set_usb() {
}

void logic_main_set_flash() {
}

void logic_main_set_keyboard() {
}

void logic_main_set_display() {
}

/**
 * Main loop for usb, data etc.
 * @param osMutexId_t Mutex for the GUI
 * @param osMutexId_t Mutex for DataBus
 */
void logic_main_loop()
{
	GUI_State ui_state = 0;

	// temp holder for key
	uint8_t key[16];

	// get latest gui state
	while(1)
	{
		// get latest GUI state
		ui_state = getGuiState();

		// run state machine
		switch(ui_state)
		{
		case GUI_STARTUP:
			// start RNG and Keystore task running.
			if(rnga != 0x00) {
				rnga->startRunning();
			}

			// move to next state
			setGuiState(GUI_STANDBY);
			break;
		case GUI_STANDBY:
			// waiting for something to happen
			break;
		case GUI_ERASING:
			// quick erase all keys
			ks->eraseAllKeys();
			setGuiState(GUI_ERASING_DONE);
			break;
		case GUI_GENERATING_KEYS:
			// for all keys
			for(uint8_t i=0; i<9; i++) {
				// request key, wait for key
				uint8_t key[16];
				rnga->generate_random_string();
				while(rnga->getGeneratedKey(&key[0], 16) < 0 ) {
					// sleep
					osDelay(50);
				}
				// finally, store key
				ks->setKey(i, &key[0]);
			}
			// generate new keys
			ks->storeKeys();
			// new state
			setGuiState(GUI_GENERATING_KEYS_DONE);
			break;
		case GUI_SEND_PASSWORD_1:
			// get temp copy of key from keystore
			ks->getKey(1, &key[0]);
			// send specified key to USB
			send_char_array_to_keyboard(&key[0], 16);
			setGuiState(GUI_STANDBY);
			break;
		case GUI_SEND_PASSWORD_2:
			// get temp copy of key from keystore
			ks->getKey(2, &key[0]);
			// send specified key to USB
			send_char_array_to_keyboard(&key[0], 16);
			setGuiState(GUI_STANDBY);
			break;
		case GUI_SEND_PASSWORD_3:
			// get temp copy of key from keystore
			ks->getKey(3, &key[0]);
			// send specified key to USB
			send_char_array_to_keyboard(&key[0], 16);
			setGuiState(GUI_STANDBY);
			break;
		case GUI_SEND_PASSWORD_4:
			// get temp copy of key from keystore
			ks->getKey(4, &key[0]);
			// send specified key to USB
			send_char_array_to_keyboard(&key[0], 16);
			setGuiState(GUI_STANDBY);
			break;
		case GUI_SEND_PASSWORD_5:
			// get temp copy of key from keystore
			ks->getKey(5, &key[0]);
			// send specified key to USB
			send_char_array_to_keyboard(&key[0], 16);
			setGuiState(GUI_STANDBY);
			break;
		case GUI_SEND_PASSWORD_6:
			// get temp copy of key from keystore
			ks->getKey(6, &key[0]);
			// send specified key to USB
			send_char_array_to_keyboard(&key[0], 16);
			setGuiState(GUI_STANDBY);
			break;
		case GUI_SEND_PASSWORD_7:
			// get temp copy of key from keystore
			ks->getKey(7, &key[0]);
			// send specified key to USB
			send_char_array_to_keyboard(&key[0], 16);
			setGuiState(GUI_STANDBY);
			break;
		case GUI_SEND_PASSWORD_8:
			// get temp copy of key from keystore
			ks->getKey(8, &key[0]);
			// send specified key to USB
			send_char_array_to_keyboard(&key[0], 16);
			setGuiState(GUI_STANDBY);
			break;
		case GUI_SEND_PASSWORD_9:
			// get temp copy of key from keystore
			ks->getKey(9, &key[0]);
			// send specified key to USB
			send_char_array_to_keyboard(&key[0], 16);
			setGuiState(GUI_STANDBY);
			break;
		case GUI_SYSTEM_FAULT:
			setGuiState(GUI_STARTUP);
			NVIC_SystemReset();
			// perform soft reset
			break;
		case GUI_SLEEP:
			// enter sleep mode
			break;
		}


		// sleep thread
		osDelay(10);
	}
}
