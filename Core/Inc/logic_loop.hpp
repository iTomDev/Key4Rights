/*
 * logic_loop.hpp
 *
 *  Created on: Sep 19, 2022
 *      Author: Tom
 */

#ifndef INC_LOGIC_LOOP_HPP_
#define INC_LOGIC_LOOP_HPP_

void logic_main_loop();
void logic_main_init();
void logic_main_set_rng(RNGDEV *dev);
void logic_main_set_rng_alt(RNGDEV *dev);
void logic_main_set_keystore(Keystore *ksin);


#endif /* INC_LOGIC_LOOP_HPP_ */
