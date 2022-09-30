/*
 * internalrng.hpp
 *
 *  Created on: Sep 15, 2022
 *      Author: Tom
 */

#ifndef INC_INTERNALRNG_HPP_
#define INC_INTERNALRNG_HPP_



#include <main.hpp>
#include "rngdev.hpp"
#include "stdint.h"
#include "_core_system_config.h"
#ifdef STM32H7
#include "stm32h7xx_hal_flash_ex.h"
#endif
#ifdef STM32F103
#include "stm32f1xx_hal_flash_ex.h"
#endif
#ifdef STM32F217
#include "stm32f2xx_hal_flash_ex.h"
#endif

class InternalRNG : RNGDEV {
protected:
	RNG_HandleTypeDef *localhrng;
	uint8_t generate_internal_random_value(RNG_HandleTypeDef *hrng);
public:
	void internal_rng_init(RNG_HandleTypeDef *hrng);
	void rtos_rng_update();
};




#endif /* INC_INTERNALRNG_HPP_ */
