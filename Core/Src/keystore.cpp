/*
 * keystore.cpp
 *
 *  Created on: Sep 13, 2022
 *      Author: Tom
 */

#include "stdint.h"
#include "keystore.hpp"
#include "string.h"
#include "cmsis_os2.h"

/**
 * Thread control
 */

Keystore::Keystore() {
}

Keystore::~Keystore() {
}

void Keystore::init(){} // placeholder function - leave blank

void Keystore::rtos_keystore_update() {
	// placeholder
}
uint8_t Keystore::getRunningState(){}
void Keystore::startRunning(){}

/**
 * Key access and removal
 */

void Keystore::getKey(uint8_t keyId, uint8_t *dataout) {
	// if keys load/save not in progress
	if (rtos_load_keys == 0 && rtos_save_keys == 0) {
		switch(keyId) {
			case 1:
				// copy key into output
				memcpy(dataout, key1, 16);
				return;
				break;
			case 2:
				memcpy(dataout, key2, 16);
				return;
				break;
			case 3:
				memcpy(dataout, key3, 16);
				return;
				break;
			case 4:
				memcpy(dataout, key4, 16);
				return;
				break;
			case 5:
				memcpy(dataout, key5, 16);
				return;
				break;
			case 6:
				memcpy(dataout, key6, 16);
				return;
				break;
			case 7:
				memcpy(dataout, key7, 16);
				return;
				break;
			case 8:
				memcpy(dataout, key8, 16);
				return;
				break;
			case 9:
				memcpy(dataout, key9, 16);
				return;
				break;
		}
	}
}

void Keystore::setKey(uint8_t keyId, uint8_t *datain) {
	// if keys load/save not in progress
	if (rtos_load_keys == 0 && rtos_save_keys == 0) {
		switch(keyId) {
			case 1:
				// copy key into output
				memcpy(key1, datain, 16);
				return;
				break;
			case 2:
				memcpy(key2, datain, 16);
				return;
				break;
			case 3:
				memcpy(key3, datain, 16);
				return;
				break;
			case 4:
				memcpy(key4, datain, 16);
				return;
				break;
			case 5:
				memcpy(key5, datain, 16);
				return;
				break;
			case 6:
				memcpy(key6, datain, 16);
				return;
				break;
			case 7:
				memcpy(key7, datain, 16);
				return;
				break;
			case 8:
				memcpy(key8, datain, 16);
				return;
				break;
			case 9:
				memcpy(key9, datain, 16);
				return;
				break;
		}
	}
}

int8_t Keystore::eraseKey(uint8_t keyId)  {
	uint8_t zeros[16];
	for(uint8_t i=0; i<16; i++){zeros[i] = 0;}
	// if keys load/save not in progress
	if (rtos_load_keys == 0 && rtos_save_keys == 0) {
		switch(keyId) {
			case 1:
				// copy key into output
				memcpy(key1, zeros, 16);
				return 0;
				break;
			case 2:
				memcpy(key2, zeros, 16);
				return 0;
				break;
			case 3:
				memcpy(key3, zeros, 16);
				return 0;
				break;
			case 4:
				memcpy(key4, zeros, 16);
				return 0;
				break;
			case 5:
				memcpy(key5, zeros, 16);
				return 0;
				break;
			case 6:
				memcpy(key6, zeros, 16);
				return 0;
				break;
			case 7:
				memcpy(key7, zeros, 16);
				return 0;
				break;
			case 8:
				memcpy(key8, zeros, 16);
				return 0;
				break;
			case 9:
				memcpy(key9, zeros, 16);
				return 0;
				break;
		}
	}
	rtos_save_keys = 1;
	return 0;
}

int8_t Keystore::eraseAllKeys() {
	uint8_t zeros[16];
	for(uint8_t i=0; i<16; i++){zeros[i] = 0;}
	memcpy(key1, zeros, 16);
	memcpy(key2, zeros, 16);
	memcpy(key3, zeros, 16);
	memcpy(key4, zeros, 16);
	memcpy(key5, zeros, 16);
	memcpy(key6, zeros, 16);
	memcpy(key7, zeros, 16);
	memcpy(key8, zeros, 16);
	memcpy(key9, zeros, 16);
	rtos_save_keys = 1;
	return 0;
}


/**
 * Load or save to hardware
 */


void Keystore::storeKeys() {
	rtos_save_keys = 1;
}

void Keystore::loadKeys() {
	rtos_load_keys = 1;
}

