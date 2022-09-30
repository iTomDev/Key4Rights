/*
 * crypto_storage.h
 *
 *  Created on: 29 Jul 2022
 *      Author: Tom
 */

#ifndef INC_INTERNAL_STORAGE_H_
#define INC_INTERNAL_STORAGE_H_

#include "stdint.h"

#ifdef INTERNAL_STORAGE

void crypto_storage_demo();

void storage_init();
int store_pwd1(uint16_t *key);
int load_pwd1(uint16_t *key);
int store_pwd2(uint16_t *key);
int load_pwd2(uint16_t *key);
void erase_block(uint8_t sector);
void erase_block_default();

void Flash_Error_Handler(uint8_t *ptr, uint32_t len);
static void MPU_Config(void);
#endif

#endif /* INC_INTERNAL_STORAGE_H_ */
