/*
 * test_basic.cpp
 *
 *  Created on: Sep 22, 2022
 *      Author: Tom
 */

#include "_core_system_config.h"
#ifdef STM32H7
#include "stm32h7xx_hal.h"
#endif
#ifdef STM32F103
#include "stm32f1xx_hal.h"
#endif
#ifdef STM32F217
#include "stm32f2xx_hal.h"
#endif
#include "stdint.h"
#include "cmsis_os2.h"

uint8_t test_basic_led_state = 0;

// pass
void test_basic_flash_led() {
	//
	#define LD3_Pin GPIO_PIN_14
	#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)

	if(test_basic_led_state > 0) {
		HAL_GPIO_WritePin(GPIOB, LD3_Pin, (GPIO_PinState) 0);
		test_basic_led_state = 0;
	}
	else {
		HAL_GPIO_WritePin(GPIOB, LD3_Pin, (GPIO_PinState) 1);
		test_basic_led_state = 1;
	}
	osDelay(200);
}

