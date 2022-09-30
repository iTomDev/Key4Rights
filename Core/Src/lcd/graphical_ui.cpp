/*
 * graphical_ui.c
 *
 *  Created on: Aug 15, 2022
 *      Author: Tom
 */

/**
 * Helper functions
 */

#include "swd_tools.h"
#include "lcd/fsl_SSD1306_I2C.h"
#include "lcd/graphical_ui.hpp"
//  RTOS
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "gui_state.hpp"
#include "i2c_tools.hpp"

/**
 * private gui function
 */




uint8_t _gui_startup(uint8_t animation_state)
{
	// display a startup logo for a period
	OLED_Clear();
	OLED_Set_Text(23, 16, kOLED_Pixel_Set, "Development", 2); // x,y
	OLED_Set_Text(42, 35, kOLED_Pixel_Set, "Build", 2); // x,y
	return animation_state;
}

uint8_t _gui_standby(uint8_t animation_state)
{
	OLED_Clear();

	// play animation
	OLED_Copy_Image(&logo_nxp[0], sizeof(logo_nxp));
	OLED_Refresh();
	return animation_state;
}

uint8_t _gui_send_password_n(uint8_t animation_state, uint8_t passwordNo)
{
	OLED_Clear();
	switch(passwordNo)
	{
	case 1:
		OLED_Set_Text(28, 16, kOLED_Pixel_Set, "Key 1", 3); // x,y
		OLED_Set_Text(32, 42, kOLED_Pixel_Set, "Sent", 3); // x,y
		break;
	case 2:
		OLED_Set_Text(28, 16, kOLED_Pixel_Set, "Key 2", 3); // x,y
		OLED_Set_Text(32, 42, kOLED_Pixel_Set, "Sent", 3); // x,y
		break;
	case 3:
		OLED_Set_Text(28, 16, kOLED_Pixel_Set, "Key 3", 3); // x,y
		OLED_Set_Text(32, 42, kOLED_Pixel_Set, "Sent", 3); // x,y
		break;
	}
	OLED_Refresh();
	return animation_state;
}

uint8_t _gui_hold_for_erase(uint8_t animation_state)
{
	OLED_Clear();
	OLED_Set_Text(28, 18, kOLED_Pixel_Set, "Hold To", 2); // x,y
	OLED_Set_Text(38, 42, kOLED_Pixel_Set, "Erase", 2); // x,y
	OLED_Refresh();
	return animation_state;
}

uint8_t _gui_erasing(uint8_t animation_state)
{
	switch (animation_state)
	{
	case 2:
		OLED_Clear();
		// generating keys
		OLED_Set_Text(6, 30, kOLED_Pixel_Set, "Erasing", 3); // x,y
		// loading blob (0)
		OLED_Set_Pixel (53, 58, 1); OLED_Set_Pixel (54, 58, 1); OLED_Set_Pixel (55, 58, 1);
		OLED_Set_Pixel (53, 59, 1); OLED_Set_Pixel (54, 59, 1); OLED_Set_Pixel (55, 59, 1);
		OLED_Set_Pixel (53, 60, 1); OLED_Set_Pixel (54, 60, 1); OLED_Set_Pixel (55, 60, 1);
		break;
	case 4:
		OLED_Clear();
		// generating keys
		OLED_Set_Text(6, 30, kOLED_Pixel_Set, "Erasing", 3); // x,y
		// loading blob (1)
		OLED_Set_Pixel (63, 58, 1); OLED_Set_Pixel (64, 58, 1); OLED_Set_Pixel (65, 58, 1);
		OLED_Set_Pixel (63, 59, 1); OLED_Set_Pixel (64, 59, 1); OLED_Set_Pixel (65, 59, 1);
		OLED_Set_Pixel (63, 60, 1); OLED_Set_Pixel (64, 60, 1); OLED_Set_Pixel (65, 60, 1);
		break;
	case 6:
		OLED_Clear();
		animation_state=0;
		// generating keys
		OLED_Set_Text(6, 30, kOLED_Pixel_Set, "Erasing", 3); // x,y
		// loading blob (1)
		OLED_Set_Pixel (73, 58, 1); OLED_Set_Pixel (74, 58, 1); OLED_Set_Pixel (75, 58, 1);
		OLED_Set_Pixel (73, 59, 1); OLED_Set_Pixel (74, 59, 1); OLED_Set_Pixel (75, 59, 1);
		OLED_Set_Pixel (73, 60, 1); OLED_Set_Pixel (74, 60, 1); OLED_Set_Pixel (75, 60, 1);
		break;
	}
	animation_state++;

	OLED_Refresh();
	return animation_state;
}


uint8_t _gui_generating_keys(uint8_t animation_state)
{
	switch (animation_state)
	{
	case 2:
		OLED_Clear();
		// generating keys
		OLED_Set_Text(23, 16, kOLED_Pixel_Set, "Generate", 2); // x,y
		OLED_Set_Text(42, 35, kOLED_Pixel_Set, "Keys", 2); // x,y
		// loading blob (0)
		OLED_Set_Pixel (53, 58, 1); OLED_Set_Pixel (54, 58, 1); OLED_Set_Pixel (55, 58, 1);
		OLED_Set_Pixel (53, 59, 1); OLED_Set_Pixel (54, 59, 1); OLED_Set_Pixel (55, 59, 1);
		OLED_Set_Pixel (53, 60, 1); OLED_Set_Pixel (54, 60, 1); OLED_Set_Pixel (55, 60, 1);
		break;
	case 4:
		OLED_Clear();
		// generating keys
		OLED_Set_Text(23, 16, kOLED_Pixel_Set, "Generate", 2); // x,y
		OLED_Set_Text(42, 35, kOLED_Pixel_Set, "Keys", 2); // x,y
		// loading blob (1)
		OLED_Set_Pixel (63, 58, 1); OLED_Set_Pixel (64, 58, 1); OLED_Set_Pixel (65, 58, 1);
		OLED_Set_Pixel (63, 59, 1); OLED_Set_Pixel (64, 59, 1); OLED_Set_Pixel (65, 59, 1);
		OLED_Set_Pixel (63, 60, 1); OLED_Set_Pixel (64, 60, 1); OLED_Set_Pixel (65, 60, 1);
		break;
	case 6:
		OLED_Clear();
		animation_state=0;
		// generating keys
		OLED_Set_Text(23, 16, kOLED_Pixel_Set, "Generate", 2); // x,y
		OLED_Set_Text(42, 35, kOLED_Pixel_Set, "Keys", 2); // x,y
		// loading blob (1)
		OLED_Set_Pixel (73, 58, 1); OLED_Set_Pixel (74, 58, 1); OLED_Set_Pixel (75, 58, 1);
		OLED_Set_Pixel (73, 59, 1); OLED_Set_Pixel (74, 59, 1); OLED_Set_Pixel (75, 59, 1);
		OLED_Set_Pixel (73, 60, 1); OLED_Set_Pixel (74, 60, 1); OLED_Set_Pixel (75, 60, 1);
		break;
	}
	animation_state++;
	OLED_Refresh();
	return animation_state;
}


/**
 *  Primary entry functions
 */

/**
 * Initialise the display
 * @param I2C_HandleTypeDef I2C device
 */
int8_t os_gui_init(I2C_HandleTypeDef I2cHandle)
{
	i2c1_init(&I2cHandle);
	OLED_Init(I2cHandle);
	OLED_Refresh();
	OLED_Clear();
	return 0;
}

/**
 * Display thread
 * @param osMutexId ID of GUI mutex
 */
void os_gui_loop()
{
	//__IO uint32_t OsStatus = 0;
	int32_t OsStatus = 0;
	//uint8_t gui_state = 0;

	uint8_t animation_state = 0; // tracks sequences in animations etc in a given state. reset when changing gui state

	GUI_State ui_state = 0;
	//ui_state = GUI_GENERATING_KEYS;

	while(1)
	{
			// get latest state
			ui_state = getGuiState();

			// state machine to determine what to display
			switch(ui_state)
			{
			case GUI_INVALID:
				break;
			case GUI_STARTUP:
				animation_state = _gui_startup(animation_state);
				osDelay(1000);
				break;
			case GUI_STANDBY:
				animation_state = _gui_standby(animation_state);
				break;
			case GUI_SEND_PASSWORD_1:
				animation_state = _gui_send_password_n(animation_state,1);
				break;
			case GUI_SEND_PASSWORD_2:
				animation_state = _gui_send_password_n(animation_state,2);
				break;
			case GUI_SEND_PASSWORD_DONE:
				osDelay(500);
				ui_state = GUI_STANDBY;
				break;
			case GUI_HOLD_FOR_ERASE:
				animation_state = _gui_hold_for_erase(animation_state);
				break;
			case GUI_ERASING:
				animation_state =  _gui_erasing(animation_state);
				break;
			case GUI_ERASING_DONE:
				// if animation completed
				if(animation_state > 5)
				{
					osDelay(500);
					ui_state = GUI_STANDBY;
				}
				else
				{
					animation_state =  _gui_erasing(animation_state);
				}
				break;
			case GUI_GENERATING_KEYS:
				animation_state = _gui_generating_keys(animation_state);
				break;
			case GUI_GENERATING_KEYS_DONE:
				// if animation completed
				if(animation_state > 5)
				{
					osDelay(500);
					ui_state = GUI_STANDBY;
				}
				else
				{
					animation_state =  _gui_generating_keys(animation_state);
				}
				break;
			}

			osDelay(100); // 10Hz
	}// loop
}

void gui_close()
{

}
