/*
 * gui_state.cpp
 *
 *  Created on: Sep 21, 2022
 *      Author: Tom
 */

#include "gui_state.hpp"
#include "stdint.h"
#include "cmsis_os2.h"

osMutexId_t guimutex;

// global state
static GUI_State global_gui_state = 0;

void guiStateInit(osMutexId_t in) {
	guimutex = in;
}

osMutexId_t mutex_id;

GUI_State getGuiState() {
	GUI_State temp;
	if(guimutex != NULL) {
		int32_t OsStatus = 0;
		OsStatus = osMutexAcquire(guimutex, 100);
		if (OsStatus == osOK) {
			temp = global_gui_state;
			OsStatus = osMutexRelease(guimutex);
			return temp;
		}
	}
	return -1;
}

void setGuiState(GUI_State stateIn) {
	if(guimutex != NULL) {
		int32_t OsStatus = 0;
		OsStatus = osMutexAcquire(guimutex, 100);
		if (OsStatus == osOK) {
			global_gui_state = stateIn;
			OsStatus = osMutexRelease(guimutex);
		}
	}
}
