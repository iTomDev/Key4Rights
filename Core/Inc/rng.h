/*
 * rng.h
 *
 *  Created on: Jul 21, 2022
 *      Author: Tom
 */

#ifndef INC_RNG_H_
#define INC_RNG_H_

#include <main.hpp>


// error codes

int8_t init_rng(uint16_t rngtousein);

// internal use
uint8_t _generate_random_value(RNG_HandleTypeDef *hrng);

// return -1 if error
int8_t generate_random_string_8(RNG_HandleTypeDef *hrng, uint8_t* arr);
int8_t generate_random_string_16(RNG_HandleTypeDef *hrng, uint8_t* arr);

int8_t rng_fix_errors();



#endif /* INC_RNG_H_ */
