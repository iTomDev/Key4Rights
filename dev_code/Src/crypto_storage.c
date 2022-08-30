/*
 * crypto_storage.c
 *
 *  Created on: Aug 29, 2022
 *      Author: Tom
 */
// app
#include "crypto_storage.h"
// api
#include "main.h"
#include "stdint.h"



/*
struct Password {
	uint8_t length;
	uint8_t slot;
	uint8_t *keystring;
};

// password data
struct Password pwd1;
struct Password pwd2;
struct Password pwd3;*/

/************************************************************************
 * Password for USB output
 */

void password_save(struct Password *pwd)
{
	#ifdef INTERNAL_STORAGE
	switch(pwd->slot)
	{
		case 1:
			store_pwd1( *pwd->keystring);
			break;
		case 2:
			store_pwd2( *pwd->keystring);
			break;
		}
	#endif

	#ifdef EXTERNAL_STORAGE
		uint8_t tmptx[33];
		tmptx[0] = (uint8_t) pwd->length;
		// copy pwd string to temp write buffer
		for(uint8_t i=0; i < pwd->length+1; i++)
		{
			tmptx[i+1] = *(pwd->keystring)++;
		}
		// save to disk
		i2c_ATECC608B_storage_save(pwd->slot, &tmptx);
	#endif
}

void password_load(struct Password *pwd)
{
	uint8_t tmprx[33];
	i2c_ATECC608B_storage_load(pwd->slot, &tmprx);
	// extract key
	for(uint8_t i=0; i < pwd->length+1; i++)
	{
		*pwd->keystring++ = tmprx[i+1];
	}
}

void password_erase(struct Password *pwd)
{
	uint8_t tmptx[32];

	// erase local temp
	for(uint8_t i=0; i<16; i++)
	{
		*pwd->keystring++ = 0;
	}

	// erase external storage
	i2c_ATECC608B_storage_save(0, &tmptx);
	
	// clear cache if in use
}


/************************************************************************
 * Bulk data encryption
 */

void bulk_storage_derive_key()
{
	// combine SN of internal and external device, rng resul etc to tie the data better
}



