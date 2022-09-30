/*
 * test_graphical_ui.cpp
 *
 *  Created on: Sep 22, 2022
 *      Author: Tom
 */

#include "stdint.h"
#include "cmsis_os2.h"
#include "gui_state.hpp"
#include "../Inc/lcd/graphical_ui.hpp"

/**************************************************************************************************************************************
 * Display tests
 */

int8_t test_display_showAllScreens()
{
	_gui_startup(0);
	HAL_Delay(1000);

	_gui_standby(0);
	HAL_Delay(1000);

	for (uint8_t i=0; i<4; i++)
	{
		_gui_send_password_n(0, i);
		HAL_Delay(500);
	}

	_gui_hold_for_erase(0);
	HAL_Delay(1000);

	for (uint8_t i=1; i<7; i++)
	{
		_gui_erasing(i);
		HAL_Delay(500);
	}

	for (uint8_t i=1; i<7; i++)
	{
		_gui_generating_keys(i);
		HAL_Delay(500);
	}
}

void test_display_loopBehaviour()
{
	// set OS state
	osMutexId_t mutexGUIHandle;

	setGuiState(GUI_STANDBY);
	os_gui_loop();
	HAL_Delay(1000);

	setGuiState(GUI_HOLD_FOR_ERASE);
	os_gui_loop();
	HAL_Delay(1000);

	setGuiState(GUI_ERASING);
	os_gui_loop();
	HAL_Delay(1000);

	setGuiState(GUI_ERASING_DONE);
	os_gui_loop();
	HAL_Delay(1000);

	setGuiState(GUI_GENERATING_KEYS);
	os_gui_loop();
	HAL_Delay(1000);

	setGuiState(GUI_GENERATING_KEYS_DONE);
	os_gui_loop();
	HAL_Delay(1000);

	setGuiState(GUI_SEND_PASSWORD_1);
	os_gui_loop();
	HAL_Delay(1000);

	setGuiState(GUI_SEND_PASSWORD_DONE);
	os_gui_loop();
	HAL_Delay(1000);

	setGuiState(GUI_SLEEP);
	os_gui_loop();
	HAL_Delay(1000);

	setGuiState(GUI_SYSTEM_FAULT);
	os_gui_loop();
	HAL_Delay(1000);

	// call display
	//

}

