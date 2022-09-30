/*
 * graphical_ui.hpp
 *
 *  Created on: Sep 22, 2022
 *      Author: Tom
 */

#ifndef INC_LCD_GRAPHICAL_UI_HPP_
#define INC_LCD_GRAPHICAL_UI_HPP_


// api
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "_core_system_config.h"
// app
#include "keypad_ui.hpp"

void lcd_demo();
int8_t os_gui_init(I2C_HandleTypeDef I2cHandle);
void os_gui_loop();

#ifdef LIVE_UNIT_TESTING
uint8_t _gui_startup(uint8_t animation_state);
uint8_t _gui_standby(uint8_t animation_state);
uint8_t _gui_send_password_n(uint8_t animation_state, uint8_t passwordNo);
uint8_t _gui_hold_for_erase(uint8_t animation_state);
uint8_t _gui_erasing(uint8_t animation_state);
uint8_t _gui_generating_keys(uint8_t animation_state);

#endif



#endif /* INC_LCD_GRAPHICAL_UI_HPP_ */
