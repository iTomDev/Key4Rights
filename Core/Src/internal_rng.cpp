/*
 * internal_rng.cpp
 *
 *  Created on: Sep 15, 2022
 *      Author: Tom
 */

#include "internalrng.hpp"
#include "delay_tools.h"

/* Internal RNG
 * Generate a random number. Generates one number
 */
uint8_t InternalRNG::generate_internal_random_value(RNG_HandleTypeDef *hrng)
{
	uint8_t rand;

	if(hrng->State == HAL_RNG_STATE_READY)
	{
		HAL_RNG_GenerateRandomNumber_IT( (RNG_HandleTypeDef *) hrng);
		delayms(10);
		if(hrng->State == HAL_RNG_STATE_READY)
		{
			rand = (uint8_t) HAL_RNG_ReadLastRandomNumber( (RNG_HandleTypeDef *) hrng);
		}
	}
	return rand;
}

void InternalRNG::rtos_rng_update() {
	// general random number if requested
	if(getRNGState() == 1) {
		// request
		uint8_t in[16];
		uint8_t out[16];
		uint8_t rand;
		// fill buffer with random numbers under 248
		for (uint8_t i=0; i<16; i++) {
			do {
				// collect 16
				rand = generate_internal_random_value(localhrng);
				// reject numbers over 247
			}
			while (rand > 247); // loop until one meets the spec
			in[i] = rand;
		}
		// convert
		uint8ToAlphanumericMap(&in[0], &out[0], 16);
		// copy to output buffer
		setGeneratedKey(&out[0], 16);
		// set task as done
		setRNGState(2);
	}
}


void InternalRNG::internal_rng_init(RNG_HandleTypeDef *hrngin) {
	*localhrng = *hrngin;
}

/*
#ifdef SWD_DEBUG
	// send the values back for checking
	SWD_Write_Uint8_Array(&rand16, 8);
	#endif
	return 0;
*/
