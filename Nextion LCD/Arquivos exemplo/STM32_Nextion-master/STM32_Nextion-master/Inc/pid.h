/*
 * pid.h
 *
 *  Created on: 08-May-2018
 *      Author: medprime
 */

#ifndef PID_H_
#define PID_H_

float PID_Calculate(float Error);
void PID_Reset_Integration_Error();


#endif /* PID_H_ */
