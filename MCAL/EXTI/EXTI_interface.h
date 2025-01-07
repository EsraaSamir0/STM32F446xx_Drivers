/*
 * EXTI_interface.h
 *
 *  Created on: Dec 9, 2024
 *      Author: User
 */

#ifndef EXTI_EXTI_INTERFACE_H_
#define EXTI_EXTI_INTERFACE_H_
#include <stdint.h>
#include <stdio.h>
#include "../../Inc/ErrorTypes.h"
#include "../../Inc/Bit_Operations.h"
#include "../../Inc/F4_Registers.h"

#define EXTI     ((EXTI_Reg_t*) EXTI_BASE_ADDRESS)

typedef enum {
	EXTI0,
	EXTI1,
	EXTI2,
	EXTI3,
	EXTI4,
	EXTI5,
	EXTI6,
	EXTI7,
	EXTI8,
	EXTI9,
	EXTI10,
	EXTI11,
	EXTI12,
	EXTI13,
	EXTI14,
	EXTI15
}EXTI_Line_t;

typedef enum {
	FALLING,
	RISING,
	ON_CHANGE
}TriggerType_t;

typedef struct {
	EXTI_Line_t EXTI_LINE;
	TriggerType_t TriggerType;
	void (*CallBackFunc)(void);
}EXTI_Config_t;

uint8_t EXTI_Init(EXTI_Config_t* EXTI_Config);
uint8_t EXTI_Disable(EXTI_Line_t EXTI_LINE);
uint8_t EXTI_ClearPendingFlag(EXTI_Line_t EXTI_LINE);
uint8_t EXTI_ReadPendingFlag(EXTI_Line_t EXTI_LINE);
uint8_t EXTI_TriggerSoftwareInterrupt(EXTI_Line_t EXTI_LINE);
uint8_t EXTI_SetCallBack(EXTI_Line_t EXTI_LINE, void (*CallBackFunc)(void));


#endif /* EXTI_EXTI_INTERFACE_H_ */
