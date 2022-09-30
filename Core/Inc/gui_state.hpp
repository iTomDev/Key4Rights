/*
 * gui_state.hpp
 *
 *  Created on: Sep 21, 2022
 *      Author: Tom
 */

#ifndef INC_GUI_STATE_HPP_
#define INC_GUI_STATE_HPP_


#include "stdint.h"
#include "cmsis_os2.h"

typedef int8_t GUI_State;

#define GUI_INVALID						-1
#define GUI_STARTUP						0
#define GUI_STANDBY						1
#define GUI_HOLD_FOR_ERASE				2
#define GUI_ERASING						3
#define GUI_ERASING_DONE				4
#define GUI_GENERATING_KEYS				5
#define GUI_GENERATING_KEYS_DONE		6
#define GUI_SEND_PASSWORD_1				51
#define GUI_SEND_PASSWORD_2				52
#define GUI_SEND_PASSWORD_3				53
#define GUI_SEND_PASSWORD_4				54
#define GUI_SEND_PASSWORD_5				55
#define GUI_SEND_PASSWORD_6				56
#define GUI_SEND_PASSWORD_7				57
#define GUI_SEND_PASSWORD_8				58
#define GUI_SEND_PASSWORD_9				59
#define GUI_SEND_PASSWORD_DONE			60
#define GUI_SLEEP						98
#define GUI_SYSTEM_FAULT				99


// concurrent accessors
void guiStateInit(osMutexId_t in);
GUI_State getGuiState();
void setGuiState(GUI_State in);


#endif /* INC_GUI_STATE_HPP_ */
