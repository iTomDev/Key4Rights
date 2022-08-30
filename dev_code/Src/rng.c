/*
 * rng.c
 *
 *  Created on: Jul 21, 2022
 *      Author: Tom
 */

#include "rng.h"
#include "swd_tools.h"

uint16_t rngtouse = 608;
uint8_t rngrepairstage = 0;
uint8_t rngerrorcount = 0;

// 0 	internal
// 608	ATECC608
int8_t init_rng(uint16_t rngtousein)
{
	rngtouse = rngtousein;
	return 0;
}

/****************************************************************************************
 * Internal RNG
 * Generate a random number. Generates one number
 */
uint8_t _generate_random_value(RNG_HandleTypeDef *hrng)
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

/*
 * Generate a random string of characters. Length 8.
 * Usage:
 * uint8_t key[8];
 * generate_random_string_8(&hrng, &key);
 * HAL_Delay(100)
 */
int8_t generate_random_string_8(RNG_HandleTypeDef *hrng, uint8_t* arr)
{
	uint8_t rand8[8] = "00000000";

	// fill array with random values
	#ifdef INTERNAL_RNG
	for (uint8_t i=0; i<8; i++)
	{
		rand8[i] = _generate_random_value(hrng);
	}
	#endif
	#ifdef EXTERNAL_RNG
	// returns 32 bytes
	uint8_t rand[32];
	if(i2c_ATECC608B_get_random(&rand)==0x00)
	{
		//
		for(uin8_t i=0; i<8; i++)
		{
			rand8[i] = rand[i];
		}
	}
	else
	{
		// return rng err
	}
	#endif

	// process random values into characters
	for(uint8_t i=0; i<8; i++)
	{
		// we need random characters and numbers for compatibility ideally
		// 26*+10 = 62, 62 * 4 = 248
		uint8_t rand = 0;
		do
		{
			if(rand < 249)
			{
				rand = rand % 64;
			}
		}
		while(rand == 0);

		rand8[i] = rand;
	}

	#ifdef SWD_DEBUG
	// send the values back for checking
	SWD_Write_Uint8_Array(&rand8, 8);
	#endif

	return 0;
}


/*
 * Generate a random string of characters. Length 8.
 * Usage:
 * uint8_t key[8];
 * generate_random_string_16(&hrng, &key);
 * HAL_Delay(100);
 */
int8_t generate_random_string_16(RNG_HandleTypeDef *hrng, uint8_t* arr)
{
	uint8_t rand16[16] = "0000000000000000";

	// fill array with random values
	#ifdef INTERNAL_RNG
	if(rngtouse==0)
	{
	for (uint8_t i=0; i<16; i++)
	{
		rand16[i] = _generate_random_value(hrng);
	}
	}
	#endif
	#ifdef EXTERNAL_RNG
	if(rngtouse==608)
	{
		// returns 32 bytes
		uint8_t rand[32];
		if(i2c_ATECC608B_get_random(&rand)==0x00)
		{
			//
			for(uin8_t i=0; i<16; i++)
			{
				rand16[i] = rand[i];
			}
		}
		else
		{
			// return rng err
		}
	}
	#endif

	// process random values into characters
	for(uint8_t i=0; i<16; i++)
	{
		// we need random characters and numbers for compatibility ideally
		// 26*+10 = 62, 62 * 4 = 248
		uint8_t rand = 0;
		do
		{
			if(rand < 249)
			{
				rand = rand % 64;
			}
		}
		while(rand == 0);
		rand16[i] = rand;
	}

	#ifdef SWD_DEBUG
	// send the values back for checking
	SWD_Write_Uint8_Array(&rand16, 8);
	#endif
	return 0;
}

// try internal if external fails
int8_t rng_fix_errors()
{
	switch(rngrepairstage)
	{
	// step 1 call error handler for device
	case 0:
		rngrepairstage++;
		// step 2 try switching device
		break;
	case 1:
		rngerrorcount++;
		if (rngerrorcount > 25)
		{
			rngerrorcount = 0;
			if (rngtouse==608)
			{
				rngtouse=0;
			}
			else
			{
				rngtouse=608;
			}
			rngrepairstage++;
		}
		break;
	case 2:
		rngrepairstage=2;
		// step 3: rng doesnt work.
		// output "insecure" as key on all channels
	}
}


