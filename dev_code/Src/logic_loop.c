/*
 * logic_loop.c
 *
 *  Created on: Aug 24, 2022
 *      Author: Tom
 */
//

// App
//#include "internal_storage.h"
#include "main.h"
#include "GUI_State.h"
#include "_core_system_config.h"
#include "crypto_storage.h"
#include "rng.h"
#include "main.h"
#include "delay_tools.h"
// api
#include "string.h"
#include "cmsis_os.h"
#include "usb_device.h"

/*
uint16_t key1[8];
uint16_t key2[8];
uint16_t decoykey1[8];
uint16_t decoykey2[8];
uint16_t bulkkey[8]; // 128 bit key
//uint16_t mode[8];

struct UserData{
		uint16_t *key1; // string of 8 characters
		uint16_t *key2;
		uint16_t *decoykey1;
		uint16_t *decoykey2;
		uint16_t *bulkkey; // 128 bit key
		uint16_t mode;
		uint16_t pwd_length_selection;
	};
*/

//struct UserData udat;

// password structs
struct Password pwd1;
struct Password pwd2;
struct Password pwd3;
uint8_t pwdstring1[16];
uint8_t pwdstring2[16];
uint8_t pwdstring3[16];

#ifdef INTERNAL_RNG
RNG_HandleTypeDef *phrng;
#endif

/**
 * Initialise main data and usb loop
 */
#ifdef INTERNAL_RNG
uint8_t logic_main_init(RNG_HandleTypeDef *hrngIn)
{
	*phrng = *hrngIn;
}
#endif
#ifdef EXTERNAL_RNG
uint8_t logic_main_init()
{

}
#endif

/**
 * Main loop for usb, data etc.
 * @param osMutexId_t Mutex for the GUI
 * @param osMutexId_t Mutex for DataBus
 */
void logic_main_loop(osMutexId_t *mutexUI_In, osMutexId_t *mutexDataBus_In)
{
	GUI_State ui_state = 0;

	// assign ID to local value. mutex is an id not a struct
	osMutexId_t mutexData;
	mutexData = mutexDataBus_In;
	osMutexId_t mutexUI;
	mutexUI = mutexUI_In;

	// get latest gui state
	while(1)
	{
		// get latest GUI state
		if (mutexUI != NULL && mutexData != NULL)
		{
			int32_t OsStatus = 0;
			OsStatus = osMutexWait(mutexUI, 100);
			if (OsStatus == osOK)
			{
				// critical data exchange only
				ui_state = global_gui_state;
				OsStatus = osMutexRelease(mutexUI);
			}
		}

		// run state machine
		switch(ui_state)
		{
		case GUI_STARTUP:
			// init
			pwd1.keystring = &pwdstring1[0];
			pwd1.length = 16;
			pwd2.keystring = &pwdstring2[0];
			pwd2.length = 16;
			pwd3.keystring = &pwdstring3[0];
			pwd3.length = 16;
			// load database
			password_load(&pwd1);
			password_load(&pwd2);
			password_load(&pwd3);
			// move to next state
			ui_state = GUI_STANDBY;
			break;
		case GUI_STANDBY:
			// waiting for something to happen
			break;
		case GUI_ERASING:
			// quick erase old keys
			#ifdef INTERNAL_STORAGE
			erase_block_default();
			#endif
			#ifdef EXTERNAL_STORAGE
			password_erase(&pwd1);
			password_erase(&pwd2);
			password_erase(&pwd3);
			#endif

			ui_state = GUI_ERASING_DONE;
			break;
		case GUI_GENERATING_KEYS:
			// generate new keys
			generate_random_string_16(phrng, pwd1.keystring);
			generate_random_string_16(phrng, pwd2.keystring);
			// store new keys to flash
			password_save(&pwd1);
			password_save(&pwd2);
			password_save(&pwd3);
			// new state
			ui_state = GUI_GENERATING_KEYS_DONE;
			break;
		case GUI_SEND_PASSWORD_1:
			// send specified key to USB
			send_char_array_to_keyboard(pwd1.keystring, pwd1.length);
			ui_state = GUI_STANDBY;
			break;
		case GUI_SEND_PASSWORD_2:
			send_char_array_to_keyboard(pwd1.keystring, pwd2.length);
			ui_state = GUI_STANDBY;
			break;
		case GUI_SYSTEM_FAULT:
			ui_state = GUI_STARTUP;
			// perform soft reset
			break;
		case GUI_SLEEP:
			// enter sleep mode
			break;
		}

		// return latest GUI state
		if (mutexUI != NULL && mutexData != NULL)
		{
			int32_t OsStatus = 0;
			OsStatus = osMutexWait(mutexUI, 100);
			if (OsStatus == osOK)
			{
				// critical data exchange only
				global_gui_state = ui_state;
				OsStatus = osMutexRelease(mutexUI);
			}
		}
		// sleep thread
		delayms(10);
	}
}
