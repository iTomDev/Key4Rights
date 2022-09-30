/*
 * _core_system_config.h
 *
 *  Created on: Aug 23, 2022
 *      Author: Tom
 */

#ifndef INC__CORE_SYSTEM_CONFIG_H_
#define INC__CORE_SYSTEM_CONFIG_H_

//#include "Core/UnitTest/LiveUnitTest.h"

// code version
#define KEY4RIGHTS_VERSION					0.1

// build type
#define LIVE_UNIT_TESTING

// print debug into to SWD terminal?
//#define SWD_DEBUG

// processor type
#define STM32H7
//#define STM32F103
//#define STM32F217

// if using the operating system. No OS isn't actually supported anyway!
#define USING_OS

// which RNG is present onboard or offboard RNG
#define INTERNAL_RNG
//#define EXTERNAL_RNG_608
#define EXTERNAL_RNG_132

// use internal or external storage.
// STMf103 doesnt seem to properly support flash storage
//#define INTERNAL_STORAGE //USE_INTERNAL_FLASH
#define EXTERNAL_STORAGE //USE_EXTERNAL_FLASH

#ifdef STM32F103
	#ifndef EXTERNAL_RNG
		#define EXTERNAL_RNG
	#endif
#endif

#endif /* INC__CORE_SYSTEM_CONFIG_H_ */
