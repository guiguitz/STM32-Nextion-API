/*
 * pid.c
 *
 *  Created on: 08-May-2018
 *      Author: medprime
 */

#include "pid.h"

#define MAX_PID_OUTPUT		        ((72000000/20000)-1)  //  clock/pwm freq
#define MAX_INTEGRATION_ERROR		(120)//120

static float PID_KP = 1000;//1000
static float PID_KI = 10;//10
static float PID_KD = 500;//500
static float PID_Integration_Error=0;
static float PID_Previous_Error=0;

float PID_Calculate(float Error)
{

	float Output;

	Output = (float) (PID_KP * Error + PID_KI * PID_Integration_Error
			+ PID_KD * (Error - PID_Previous_Error));

	if (PID_KI != 0 && Output < MAX_PID_OUTPUT)
	{
		PID_Integration_Error += Error;
		// Limit the maximum integration Error
		if (PID_Integration_Error > MAX_INTEGRATION_ERROR)
		{
			PID_Integration_Error = MAX_INTEGRATION_ERROR;
		}
		else if (PID_Integration_Error < -MAX_INTEGRATION_ERROR)
		{
			PID_Integration_Error = -MAX_INTEGRATION_ERROR;
		}
	}
	else
	{
		//PID_Integration_Error=0;
	}

// Limit the maximum Output
	if (Output > MAX_PID_OUTPUT)
	{
		Output = MAX_PID_OUTPUT;
	}
	if (Output < 0)
	{
		Output = 0;
	}

	PID_Previous_Error = Error;

	return Output;
}

void PID_Reset_Integration_Error()
{
	PID_Integration_Error=0;
}

