/*
 * graphics_image_header.c
 *
 *  Created on: Aug 21, 2022
 *      Author: Tom
 */

#include <main.hpp>

// load animated image from header file to display on screen
#define fheight 64
#define fwidth 128
uint8_t Frame[fheight][fwidth];
uint32_t frameid;


void gif_header_getNextFrame()
{
	// loop through the cols

	uint16_t frameoffset = (frameid*fwidth);
	for (uint16_t j = 0; j < 64; j++)
	{
		for (uint16_t i=0; i < 120; i++)
		{
			//Frame[j][i] = [j][i+frameoffset];
		}
	}
}
