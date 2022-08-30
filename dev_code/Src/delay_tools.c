/*
 * delay_tools.c
 *
 *  Created on: 29 Aug 2022
 *      Author: Tom
 */

#include "_core_system_config.h"
#include "main.h"

#include "cmsis_os.h"


void delayms(uint16_t ms)
{
	#ifdef USING_OS
	osDelay(10);
	#else
	HAL_Delay(10);
	#endif
}
