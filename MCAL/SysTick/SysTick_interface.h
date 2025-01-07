/*
 * SysTick_interface.h
 *
 *  Created on: Nov 16, 2024
 *      Author: User
 */

#ifndef MCAL_SYSTICK_SYSTICK_INTERFACE_H_
#define MCAL_SYSTICK_SYSTICK_INTERFACE_H_
#include <stdint.h>
#include "../../Inc/F4_Registers.h"
#include "../../Inc/Bit_Operations.h"
#include "../../Inc/ErrorTypes.h"


void SysTick_delay_ms(float time);
void SysTick_delay_us(float time);


#endif /* MCAL_SYSTICK_SYSTICK_INTERFACE_H_ */
