/*
 * ascii_keyboard.h
 *
 *  Created on: Jul 20, 2022
 *      Author: Tom
 */

#ifndef INC_ASCII_KEYBOARD_H_
#define INC_ASCII_KEYBOARD_H_

#include "usbd_def.h"
#include "usbd_hid.h"

void init_ascii_keyboard();
void send_char_to_keyboard(char x);
void send_char_array_to_keyboard(uint8_t* arr, uint8_t len);

uint8_t char_to_keyboard_case(char x);
uint8_t char_to_keyboard(char x);	// returning 0 if error, weird

#endif /* INC_ASCII_KEYBOARD_H_ */
