/*
 * GUI_State.h
 *
 *  Created on: Aug 23, 2022
 *      Author: Tom
 */

#ifndef INC_GUI_STATE_H_
#define INC_GUI_STATE_H_


typedef uint8_t GUI_State;

/*
// UI states
#define STARTUP 		0
#define CRYPTORESET 	1
#define WAIT_STATE		2
#define STATEFAULT		3
#define SLEEP_MODE		4
#define SENDKEY1		51
#define SENDKEY2		52
*/

#define GUI_STARTUP						0
#define GUI_STANDBY						1
#define GUI_HOLD_FOR_ERASE				2
#define GUI_ERASING						3
#define GUI_ERASING_DONE				4
#define GUI_GENERATING_KEYS				5
#define GUI_GENERATING_KEYS_DONE		6
#define GUI_SEND_PASSWORD_1				51
#define GUI_SEND_PASSWORD_2				52
#define GUI_SEND_PASSWORD_3				53
#define GUI_SEND_PASSWORD_4				54
#define GUI_SEND_PASSWORD_5				55
#define GUI_SEND_PASSWORD_6				56
#define GUI_SEND_PASSWORD_7				57
#define GUI_SEND_PASSWORD_8				58
#define GUI_SEND_PASSWORD_9				59
#define GUI_SEND_PASSWORD_DONE			60
#define GUI_SLEEP						98
#define GUI_SYSTEM_FAULT				99


// global state
static GUI_State global_gui_state = 0;

#endif /* INC_GUI_STATE_H_ */
