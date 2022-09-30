/*
 * keystore.hpp
 *
 *  Created on: Sep 14, 2022
 *      Author: Tom
 */

#ifndef INC_KEYSTORE_HPP_
#define INC_KEYSTORE_HPP_

#include "stdint.h"
#include "cmsis_os2.h"

class Keystore {
protected:
	uint8_t key1[16];
	uint8_t key2[16];
	uint8_t key3[16];
	uint8_t key4[16];
	uint8_t key5[16];
	uint8_t key6[16];
	uint8_t key7[16];
	uint8_t key8[16];
	uint8_t key9[16];
	// thread todo
	uint8_t rtos_load_keys;
	uint8_t rtos_save_keys;
	uint8_t rtos_key_to_get;
public:
	/**
	 * Thread control
	 */
	Keystore();
	~Keystore();
	// to set a mutex
	virtual void init();
	// os sub-loop
	virtual void rtos_keystore_update();
	// thread
	virtual uint8_t getRunningState();
	virtual void startRunning();
	/**
	 * Key access and removal
	 */
	void getKey(uint8_t keyId, uint8_t *dataout);
	void setKey(uint8_t keyId, uint8_t *dataout);
	int8_t eraseKey(uint8_t keyId);
	int8_t eraseAllKeys();

	/**
	 * Load or save to hardware
	 */
	void storeKeys();
	void loadKeys();
};


#endif /* INC_KEYSTORE_HPP_ */
