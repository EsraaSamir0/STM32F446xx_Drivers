/*
 * SYSCFG_interface.h
 *
 *  Created on: Dec 11, 2024
 *      Author: User
 */

#ifndef SYSCFG_SYSCFG_INTERFACE_H_
#define SYSCFG_SYSCFG_INTERFACE_H_
#include <stdint.h>
#include <stdio.h>
#include "../../Inc/ErrorTypes.h"
#include "../../Inc/Bit_Operations.h"
#include "../../Inc/F4_Registers.h"
#include "../GPIO/GPIO_interface.h"
#include "../EXTI/EXTI_interface.h"

#define SYSCFG      ((SYSCFG_Reg_t*)SYSCFG_BASE_ADDRESS)

uint8_t SYSCFG_SetEXTIPort(EXTI_Line_t EXTI_LINE, Port_t Port);


#endif /* SYSCFG_SYSCFG_INTERFACE_H_ */
