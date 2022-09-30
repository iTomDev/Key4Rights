/*
 * rng.cpp
 *
 *  Created on: Sep 13, 2022
 *      Author: Tom
 */

#include "stdint.h"
#include "string.h"
#include "../Inc/rngdev.hpp"
#include "cmsis_os2.h"

/**
 *  Thread setup and start
 */

void RNGDEV::init(){} // placeholder function - leave blank

void RNGDEV::rtos_rng_update() {} // placeholder function - leave blank

// thread
uint8_t RNGDEV::getRunningState(){}
void RNGDEV::startRunning(){}

/**
 * State control. Threaded program, but it takes time to generate keys
 */

uint8_t  RNGDEV::getRNGState() {
		return key_gen_state;
	}

void RNGDEV::setRNGState(uint8_t in) {
		key_gen_state = in;
	}

int8_t RNGDEV::setGeneratedKey(uint8_t *data, uint8_t len) {
	// todo
	return 0;
}

int8_t RNGDEV::getGeneratedKey(uint8_t *data, uint8_t len) {
	// check if request isd complete
	if (key_gen_state > 1) {
		// clear the request
		request_string_length = 0;
		// copy across to avoid concurrency pointer issues
		memcpy(data, keygen, len);
		key_gen_state = 0;
	}
	else {
		// wait for action to be completed
	}
	return 0;
}

void RNGDEV::generate_random_string() {
	// check if generation is in progress
	if(key_gen_state == 0) {
		request_string_length = 16;
		key_gen_state = 1;
	}
}

/**
 * Helper functions
 */

// return -1 is failed because input vector contains too many over 247s or is too short
int8_t RNGDEV::uint8ToAlphanumericMap(uint8_t *in, uint8_t *out, uint8_t input_len) {
	// process random values into characters

	// loop through the output buffer. input index can be past 15 because we reject values!
	for(uint8_t i=0; i<16; i++) {
		// only consider numbers in the range 0 to 247 inclusive
		// otherwise the probabilities of the extra numbers will be heavily skewed
		if (in[i] < 248) {
			do {
				// map 62 values to 0-61
				in[i] = in[i] % 62;
			}
			while(in[i] > 61);
			out[i] = in[i];
		}
		else
		{
			i++;
			// if we are outside the range of input values provided, reject the conversion its not fully secure
			// +1 is just so we can use >
			if(i+1 > input_len)
			{
				return -1;
			}
		}
	}
	return 0;
}


/**
 * Unit testing
 */

void TEST_uint8ToAlphanumericMap()
{
	// in: vector of 248 should out all zero vecter return length zero
	//uint8ToAlphanumericMap(uint8_t *in, uint8_t *out);

	// in:  0	61	62	124	  186
	// out: 0	61	0	0	  0
}
