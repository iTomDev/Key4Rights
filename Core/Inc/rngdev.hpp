/*
 * rng.hpp
 *
 *  Created on: Sep 14, 2022
 *      Author: Tom
 */

#ifndef INC_RNGDEV_HPP_
#define INC_RNGDEV_HPP_

#include "stdint.h"
#include "cmsis_os2.h"

class RNGDEV {
private:
	uint8_t key_gen_state; // 0 free, 1 conversion requested, 2 conversion complete
	uint8_t keygen[16]; // generated key to return
	uint8_t request_string_length = 0; // 0, 8, 16
	osMutexId_t rngmutex;
public:
	/**
	 * Thread setup and start
	 */
	virtual void init();
	virtual void rtos_rng_update();
	// thread
	virtual uint8_t getRunningState();
	virtual void startRunning();
	/**
	 * State control. Threaded program, but it takes time to generate keys
	 */
	uint8_t getRNGState();
	void setRNGState(uint8_t in);

	/**
	 * Get and set
	 */
	// does not return a value, must be collected with thread safe getter
	void generate_random_string();
	// return 0 if ok, -1 if key not ready
	int8_t setGeneratedKey(uint8_t *data, uint8_t len);
	int8_t getGeneratedKey(uint8_t *data, uint8_t len);

	/**
	 * Helper functions
	 */
	int8_t uint8ToAlphanumericMap(uint8_t *in, uint8_t *out, uint8_t input_len);
};

#endif /* INC_RNGDEV_HPP_ */
