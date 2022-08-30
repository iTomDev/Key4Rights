/*
 * logic_loop.h
 *
 *  Created on: Aug 24, 2022
 *      Author: Tom
 */

#ifndef INC_LOGIC_LOOP_H_
#define INC_LOGIC_LOOP_H_

#ifdef INTERNAL_RNG
uint8_t logic_main_init(RNG_HandleTypeDef *hrngIn);
#endif
#ifdef EXTERNAL_RNG
uint8_t logic_main_init();
#endif

void logic_main_loop(osMutexId_t *mutexUI_In, osMutexId_t *mutexData_In);

#endif /* INC_LOGIC_LOOP_H_ */
