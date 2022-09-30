/*
 * test_ataes132_spi.cpp
 *
 *  Created on: Sep 22, 2022
 *      Author: Tom
 */

#include "test_ataes132_spi.hpp"
#include "../Inc/ataes132_spi.hpp"
#include "../Inc/_core_system_config.h"

#ifdef LIVE_UNIT_TESTING




// 0 pass, -1 fail
int8_t ATAES132_SPI_Test::setUp() {
	//at132.init(hspiin, mutexin)
}

void ATAES132_SPI_Test::tearDown() {

}




/**************************************************************************************************************************************
 * Logic loop tests
 */

// test that logic loop takes and returns global state
// GUI_SYSTEM_FAULT ->  GUI_STARUP

// 456

/**************************************************************************************************************************************
 * Internal RNG tests
 */

void test_internal_rng_generateDifferentNumbers()
{
	uint8_t arr1[16];
	uint8_t arr2[16];
	int8_t pass = 0;

	// todo
	// check that 8 characters are returned
	//generate_random_string_8(test_hrng, &arr1[0]);
	//generate_random_string_8(test_hrng, &arr2[0]);


	// make sure they arent the same
	uint32_t sum1, sum2;
	for (uint8_t i=0; i<8; i++)
	{
		sum1 = sum1 + arr1[i];
		sum2 = sum2 + arr2[i];
	}
	if(sum1 == sum2){pass = -1;}

	// zero the array and sum1
	for (uint8_t i=0; i<16; i++)
	{
		arr1[i] = 0;
		arr2[i] = 0;
	}
	sum1 = 0; sum2 = 0;

	// todo
	// check that 16 characters are returned
	//generate_random_string_16(test_hrng, &arr1[0]);
	//generate_random_string_16(test_hrng, &arr2[0]);
	// make sure they arent the same
	for (uint8_t i=0; i<16; i++)
	{
		sum1 = sum1 + arr1[i];
		sum2 = sum2 + arr2[i];
	}
	if(sum1 == sum2){pass = -1;}
}

#endif

