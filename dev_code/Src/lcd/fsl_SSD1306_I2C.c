/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*----esta modificada para usar el I2C1, en caso de utilizar otro cambiar todo lo que diga I2C1 por la base del I2C usado--- */
/*---- BASADO EN fsl_SSD1306.h para SPI de NXP ------*/

#ifdef STM32F217
#include "stm32f2xx_hal.h"
#endif
#ifdef STM32F103
#include "stm32f1xx_hal.h"
#endif
#ifdef STM32H7
#include "stm32h7xx_hal.h"
#endif
#include "lcd/fsl_SSD1306_I2C.h"
//#include "fsl_gpio.h"
//#include "fsl_i2c.h"
//#include "fsl_Systick_Delay.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief OLED buffer */
static uint8_t OLED_Buffer[(OLED_WIDTH * OLED_HEIGHT) / 8];

/*******************************************************************************
 * Code
 ******************************************************************************/

I2C_HandleTypeDef I2cHandle;


static void OLED_Command (uint8_t Cmd)
{
	/*
	//#define I2C_ADDRESS			(uint16_t) (0x3C) //0x30F
	#define I2C_MEM_ADDRESS_1		(uint16_t) 0x00
	#define I2C_TIMING			0x00901954

	while(HAL_I2C_Mem_Write_DMA(&I2cHandle, (uint16_t)SSD1306_ADDRESS_1, (uint16_t) I2C_MEM_ADDRESS_1, (uint16_t) 0x02, Cmd, sizeof(Cmd))!= HAL_OK)
	{
		if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
		{
			//Error_Handler();
		}
	}*/


	uint16_t devaddy = 0x3C;
	i2c1_send(devaddy, 0x00, Cmd);

	HAL_Delay(1);
}


// sizeof(OLED_Buffer) is fixed bvut *data is variable. temp fix
static void OLED_Data (uint8_t *Data)
{
	/*
	//#define I2C_ADDRESS			(uint16_t) (0x3C) //0x30F
	#define I2C_MEM_ADDRESS_2		(uint16_t) 0x40
	#define I2C_TIMING			0x00901954

	while(HAL_I2C_Mem_Write_DMA(&I2cHandle, (uint16_t)SSD1306_ADDRESS_1, (uint16_t) I2C_MEM_ADDRESS_2, (uint16_t) 0x02, (uint8_t*)Data, sizeof(OLED_Buffer))!= HAL_OK)
	{
		if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
		{
			//Error_Handler();
		}
	}
	*/

	/*
	uint8_t aTxBuffer[1] = {0x90};
	uint16_t devaddy = 0x3D; // 0x7A;
	uint16_t regaddy = 0x40;
	while(HAL_I2C_Mem_Write(&I2cHandle, (uint16_t)devaddy, (uint16_t)regaddy, (uint16_t) 0x01, (uint8_t*)Data, 1, 10000)!= HAL_OK)
	{}*/

	/*
	for (uint16_t i=0; i<sizeof(OLED_Buffer); i++)
	{
		OLED_Buffer[i] = 0xFF;
	}
	*/

	uint16_t devaddy = 0x3C;
	//i2c_send(devaddy, 0x40, *Data);
	i2c1_send_block(devaddy, (uint16_t) 0x40, Data, sizeof(OLED_Buffer));
	//i2c_send_block(devaddy, (uint16_t) 0x40, &OLED_Buffer, sizeof(OLED_Buffer));

	//HAL_Delay(1);
}

static void OLED_Reset (void)
{
	OLED_Command(OLED_DISPLAYON);
//	for(int i=0; i<1000; i++);					//delay 1ms
	OLED_Command(OLED_DISPLAYOFF);
//	for(int i=0; i<1000; i++);					//delay 1ms
	OLED_Command(OLED_DISPLAYON);
//	for(int i=0; i<10000; i++);					//delay 10ms

}

static void OLED_Config_Display (void)
{

	OLED_Reset();

	// Turn the OLED Display off
	OLED_Command(OLED_DISPLAYOFF);

	// Configure the display for 128x64 pixels, KS0108 mode
	OLED_Command(OLED_SETDISPLAYCLOCKDIV);
	OLED_Command(0x80);
	OLED_Command(OLED_SETMULTIPLEX);
	OLED_Command(OLED_HEIGHT-1);				// LCD Height
	OLED_Command(OLED_SETDISPLAYOFFSET);
	OLED_Command(0x0);
	OLED_Command(OLED_SETSTARTLINE | 0x0);
	OLED_Command(OLED_CHARGEPUMP);
	OLED_Command(0x14);							// Use 3.3V supply to generate high voltage supply
	OLED_Command(OLED_MEMORYMODE);
	OLED_Command(0x00);
	OLED_Command(OLED_SEGREMAP | 0x1);
	OLED_Command(OLED_COMSCANDEC);
	OLED_Command(OLED_SETCOMPINS);
	OLED_Command(0x12);
	OLED_Command(OLED_SETCONTRAST);
	OLED_Command(0xCF);
	OLED_Command(OLED_SETPRECHARGE);
	OLED_Command(0xF1);
	OLED_Command(OLED_SETVCOMDETECT);
	OLED_Command(0x40);
	OLED_Command(OLED_DISPLAYALLON_RESUME);
	OLED_Command(OLED_NORMALDISPLAY);
	OLED_Command(OLED_DEACTIVATE_SCROLL);

	OLED_Command(OLED_COLUMNADDR);
	OLED_Command(0);
	OLED_Command(OLED_WIDTH-1);
	OLED_Command(OLED_PAGEADDR);
	OLED_Command(0);
	OLED_Command(OLED_HEIGHT/8-1);

	// Turn the OLED display on!
	OLED_Command(OLED_DISPLAYON);

	OLED_Command(OLED_SETLOWCOLUMN | 0x0);  // low col = 0
	OLED_Command(OLED_SETHIGHCOLUMN | 0x0);  // hi col = 0
	OLED_Command(OLED_SETSTARTLINE | 0x0); // line #0

}


static int OLED_Render_Char (uint8_t X_axis, uint8_t Y_axis, uint8_t SC, int8_t String, uint8_t Scale)
{

	uint8_t px, py;
	uint16_t start_pos;

	if ((X_axis >= OLED_WIDTH) || (Y_axis >= OLED_HEIGHT)) {
		return 1;
	}
	if (String > 127) {
		return 2;
	}
	if (Scale > 3) {
		return 3;
	}

	start_pos = ((uint8_t)String) * 7;			// Characters have a 7 row offset
	for (px=0; px<5; px++) {
		for (py=0; py<7; py++) {
			if ((font5x7[start_pos+py] >> (7-px)) & 1) {
				switch (Scale) {
				case 3:
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+2, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+2, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale)+2, SC);
					break;
				case 2:
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+1, SC);
					break;
				case 1:
				default:
					OLED_Set_Pixel(X_axis+px, Y_axis+py, SC);
					break;
				}
			}
		}
	}
	return 0;
}

void OLED_Init(I2C_HandleTypeDef I2cHandleIn)
{
	I2cHandle = I2cHandleIn;
	i2c1_init(&I2cHandle);

	/*Give the display a reset*/
	OLED_Reset();

	/* Clear the framebuffer*/
	OLED_Clear();

	/*Configure the OLED display controller*/
	OLED_Config_Display();

}

void OLED_Refresh(void)
{

	OLED_Command(0xb0);
	OLED_Command(((0&0xf0)>>4) | 0x10);
	OLED_Command((0&0x0f) | 0x01);

	OLED_Data(&OLED_Buffer[0]);

}

void OLED_Clear(void)
{

	memset(OLED_Buffer, 0, sizeof(OLED_Buffer));

}


void OLED_Fill(uint8_t Pattern)
{

	memset(OLED_Buffer, Pattern, sizeof(OLED_Buffer));

}

void OLED_Display_Mode (uint8_t Mode)
{

	if (Mode) {
		OLED_Command(OLED_INVERTDISPLAY);
	}
	else {
		OLED_Command(OLED_NORMALDISPLAY);
	}

}

void OLED_Set_Pixel (uint8_t X_axis, uint8_t Y_axis, uint8_t SC)
{

	if((X_axis >= OLED_WIDTH) || (Y_axis >= OLED_HEIGHT))
	{
		//Do nothing
	}
	else
	{
		switch(SC)
		{
			case kOLED_Pixel_Clear:
				OLED_Buffer [X_axis + (Y_axis / 8) * OLED_WIDTH] &= ~(1 << (Y_axis & 7));
				break;
			case kOLED_Pixel_Set:
				OLED_Buffer [X_axis + (Y_axis / 8) * OLED_WIDTH] |= (1 << (Y_axis & 7));
				break;
		}
	}
}

void OLED_Set_Text (uint8_t X_axis, uint8_t Y_axis, uint8_t SC, char* String, uint8_t Scale)
{
	uint16_t Cont;
	uint16_t xscaled;

	if ((X_axis >= OLED_WIDTH) || (Y_axis >= OLED_HEIGHT)) {
		//Do nothing
	}
	else
	{
		if (Scale > 3) {
			//Do nothing
		}
		else
		{

			for (Cont = 0; String[Cont] != '\0'; Cont++) {
				// Catch overflow when scaling!
				xscaled = X_axis+(Cont*5*Scale);
				if (xscaled > OLED_WIDTH)
				{
					//Do nothing
				}

				else
				{
					OLED_Render_Char(xscaled, Y_axis, SC, String[Cont], Scale);

				}
			}
		}
	}

}

void OLED_Copy_Image(const uint8_t *Img, uint16_t size){

	uint16_t CpyBuffer;

	OLED_Clear();

	for(CpyBuffer = 0; CpyBuffer < size - 1; CpyBuffer++)
	{
		OLED_Buffer[CpyBuffer] = *(Img + CpyBuffer);
	}
}
