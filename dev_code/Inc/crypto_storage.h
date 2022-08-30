/*
 * crypto_storage.h
 *
 *  Created on: Aug 29, 2022
 *      Author: Tom
 */

#ifndef CRYPTO_STORAGE_H_
#define CRYPTO_STORAGE_H_

#include "main.h"
#include "stdint.h"

struct Password {
	uint8_t length;
	uint8_t slot;
	uint8_t *keystring;
};

// functions
void password_save(struct Password *pwd);
void password_load(struct Password *pwd);
void password_erase(struct Password *pwd);

#endif /* CRYPTO_STORAGE_H_ */
