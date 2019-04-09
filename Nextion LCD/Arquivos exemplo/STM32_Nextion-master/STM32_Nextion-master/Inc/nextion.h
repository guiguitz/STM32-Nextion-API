/*
 * nextion.h
 *
 *  Created on: 02-Aug-2018
 *      Author: medprime
 */

#ifndef NEXTION_H_
#define NEXTION_H_

#include "stm32f1xx_hal.h"
#include "ring_buffer.h"
#include "string.h"

#define NEXTION_OK  1
#define NEXTION_ERR 0

typedef struct Nextion_Object_t
{
	uint8_t Page_ID;
	uint8_t Component_ID;
	char*   Name;

	void (*Push_Callback)(); //callback function ptr
	void (*Pop_Callback)();  //callback function ptr

} Nextion_Object_t;

extern UART_HandleTypeDef huart1;

uint8_t Nextion_Init();
uint8_t Nextion_Add_Object(Nextion_Object_t* PTR);
void    Nextion_Send_Command(const char* cmd);
void    Nextion_Get_Current_Page();


void Nextion_Loop();

void Nextion_Set_Text(char *object_name, char *buffer);
void Nextion_Set_Value_Float(char *object_name, float number);
void Nextion_Set_BCK_Colour(char *object_name, int colour);
void Nextion_Hide_Object(char *object_name, uint8_t hide);
void Nextion_Backlight_Brightness(uint8_t value, uint8_t overide);
void Nextion_Baud_Rate(uint32_t baud, uint8_t overide);

void Nextion_Get_Text(char *buffer);



#endif /* NEXTION_H_ */
