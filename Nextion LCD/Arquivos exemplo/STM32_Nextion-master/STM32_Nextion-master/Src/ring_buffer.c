/*
 * ring_buffer.c
 *
 *  Created on: 01-Aug-2018
 *      Author: medprime
 */



/** Implementation of read only ring buffer for uart
 *
 * data is written to buffer via uart DMA in background
 *
 *
 */


#include "ring_buffer.h"

#define BUFFER_SIZE       64  /* must be power of two */

static uint8_t RX_DMA_Buffer[BUFFER_SIZE]={0};
static UART_HandleTypeDef *huart;
static uint32_t Read_PTR;

#define WRITE_PTR (uint32_t)( BUFFER_SIZE - (huart->hdmarx->Instance->CNDTR))

void Ring_Buffer_Init(UART_HandleTypeDef *_huart)
{
	huart = _huart;
	huart->hdmarx->Instance->CNDTR = BUFFER_SIZE;
	Read_PTR = 0;
	HAL_UART_Receive_DMA(huart, RX_DMA_Buffer, BUFFER_SIZE);

}

uint8_t Ring_Buffer_Is_Full(void)
{

	return (Read_PTR == WRITE_PTR)?1:0;
}

uint8_t Ring_Buffer_Get_Char(uint8_t* data)
{
	if (WRITE_PTR == Read_PTR)
	{
		return 0;
	}
	else
	{
		*data = RX_DMA_Buffer[Read_PTR];

		Read_PTR++;

		if (Read_PTR == BUFFER_SIZE)
		{
			Read_PTR = 0;
		}

		return 1;
	}
}

uint8_t Ring_Buffer_Get_Count(void)
{
	if (WRITE_PTR >= Read_PTR)
	{
		return (WRITE_PTR - Read_PTR);
	}
	return (BUFFER_SIZE - (Read_PTR - WRITE_PTR));
}


void Ring_Buffer_Flush()
{
	Read_PTR = WRITE_PTR;
}





static uint32_t Check_PTR = 0;

uint8_t Ring_Buffer_Check_Char(uint8_t* data)
{
	if (WRITE_PTR == Check_PTR)
	{
		return 0;
	}
	else
	{
		*data = RX_DMA_Buffer[Check_PTR];

		Check_PTR++;

		if (Check_PTR == BUFFER_SIZE)
		{
		    Check_PTR = 0;
		}

		return 1;
	}
}


uint8_t Ring_Buffer_Check_Count(void)
{
	if (WRITE_PTR >= Check_PTR)
	{
		return (WRITE_PTR - Check_PTR);
	}
	return (BUFFER_SIZE - (Check_PTR - WRITE_PTR));
}


