/*
 * graphical_ui.h
 *
 *  Created on: Aug 16, 2022
 *      Author: Tom
 */

#ifndef INC_LCD_GRAPHICAL_UI_H_
#define INC_LCD_GRAPHICAL_UI_H_

// api
#include "cmsis_os.h"
#include "FreeRTOS.h"
// app
#include "keypad_ui.h"

void lcd_demo();
int8_t os_gui_init(I2C_HandleTypeDef I2cHandle);
void os_gui_loop(osMutexId mutexIn);

#endif /* INC_LCD_GRAPHICAL_UI_H_ */
