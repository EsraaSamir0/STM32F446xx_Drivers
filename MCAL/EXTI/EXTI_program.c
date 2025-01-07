/*
 * EXTI_program.c
 *
 *  Created on: Dec 9, 2024
 *      Author: User
 */
#include "EXTI_interface.h"
#include "EXTI_private.h"


void (*EXTI_CallBackFunc[EXTI_LINES_NUM])(void) = {0}; /* Array of pointers to call back functions */


uint8_t EXTI_Init(EXTI_Config_t* EXTI_Config){

	if (EXTI_Config == NULL){
		return NULL_PTR_ERR;
	}
	if ((EXTI_Config->EXTI_LINE >= EXTI0) && (EXTI_Config->EXTI_LINE <= EXTI15)){

		/*Configuring the trigger type*/
		switch (EXTI_Config->TriggerType){
		case FALLING :
			CLR_BIT(EXTI->FTSR,EXTI_Config->EXTI_LINE);
			SET_BIT(EXTI->FTSR,EXTI_Config->EXTI_LINE);
			break;
		case RISING  :
			CLR_BIT(EXTI->RTSR,EXTI_Config->EXTI_LINE);
			SET_BIT(EXTI->RTSR,EXTI_Config->EXTI_LINE);
			break;
		case ON_CHANGE :
			CLR_BIT(EXTI->FTSR,EXTI_Config->EXTI_LINE);
			CLR_BIT(EXTI->RTSR,EXTI_Config->EXTI_LINE);
			SET_BIT(EXTI->FTSR,EXTI_Config->EXTI_LINE);
			SET_BIT(EXTI->RTSR,EXTI_Config->EXTI_LINE);
			break;
		}
		/* Enabling The EXTI Line */
			EXTI->IMR |= (1 << EXTI_Config->EXTI_LINE);
		/* Setting callback function if provided in the structure */
		if (EXTI_Config->CallBackFunc != NULL) {
			EXTI_CallBackFunc[EXTI_Config->EXTI_LINE] = EXTI_Config->CallBackFunc;
		}
	}else {
		return ERROR;
	}
	return OK;
}

uint8_t EXTI_Disable(EXTI_Line_t EXTI_LINE){

	if ((EXTI_LINE >= EXTI0) && (EXTI_LINE <= EXTI15)){
		EXTI->IMR &= ~(1 << EXTI_LINE);
	}else {
		return ERROR;
	}
	return OK;
}

uint8_t EXTI_ClearPendingFlag(EXTI_Line_t EXTI_LINE){

	if ((EXTI_LINE >= EXTI0) && (EXTI_LINE <= EXTI15)){
		EXTI->PR = (1 << EXTI_LINE);
	}else {
		return ERROR;
	}
	return OK;
}

uint8_t EXTI_ReadPendingFlag(EXTI_Line_t EXTI_LINE){

	if ((EXTI_LINE >= EXTI0) && (EXTI_LINE <= EXTI15)){
		return GET_BIT(EXTI->PR, EXTI_LINE);
	}else {
		return ERROR;
	}
}

uint8_t EXTI_TriggerSoftwareInterrupt(EXTI_Line_t EXTI_LINE){

	if ((EXTI_LINE >= EXTI0) && (EXTI_LINE <= EXTI15)){
		if (GET_BIT(EXTI->IMR, EXTI_LINE) == 0) {
			return ERROR; // Line not enabled
		}
		EXTI->SWIER |= (1 << EXTI_LINE);
	}else{
		return ERROR;
	}
	return OK;
}

uint8_t EXTI_SetCallBack(EXTI_Line_t EXTI_LINE, void (*CallBackFunc)(void)){

	if (CallBackFunc == NULL){
		return NULL_PTR_ERR;
	}
	if ((EXTI_LINE >= EXTI0) && (EXTI_LINE <= EXTI15)){
		EXTI_CallBackFunc[EXTI_LINE] = CallBackFunc;
	}else {
		return ERROR;
	}
	return OK;
}

void EXTI0_IRQHandler (void){
	/*Clear pending flag*/
	EXTI->PR |= (1 << EXTI0);
	EXTI_CallBackFunc[EXTI0]();
}

void EXTI1_IRQHandler (void){
	/*Clear pending flag*/
	EXTI->PR |= (1 << EXTI1);
	EXTI_CallBackFunc[EXTI1]();
}

void EXTI2_IRQHandler (void){
	/*Clear pending flag*/
	EXTI->PR |= (1 << EXTI2);
	EXTI_CallBackFunc[EXTI2]();
}

void EXTI3_IRQHandler (void){
	/*Clear pending flag*/
	EXTI->PR |= (1 << EXTI3);
	EXTI_CallBackFunc[EXTI3]();
}

void EXTI4_IRQHandler (void){
	 /*Clear pending flag*/
	EXTI->PR |= (1 << EXTI4);
	EXTI_CallBackFunc[EXTI4]();
}

void EXTI9_5_IRQHandler (void){   /* EXTI Line[9:5] interrupts */

	if (EXTI5_FLAG == Raised){
		/*Clear pending flag, and call IRQ handler*/
		EXTI->PR |= (1 << EXTI5);
		EXTI_CallBackFunc[EXTI5]();
	}

	if (EXTI6_FLAG == Raised){
		/*Clear pending flag, and call IRQ handler*/
		EXTI->PR |= (1 << EXTI6);
		EXTI_CallBackFunc[EXTI6]();
	}

	if (EXTI7_FLAG == Raised){
		/*Clear pending flag, and call IRQ handler*/
		EXTI->PR |= (1 << EXTI7);
		EXTI_CallBackFunc[EXTI7]();
	}

	if (EXTI8_FLAG == Raised){
		/*Clear pending flag, and call IRQ handler*/
		EXTI->PR |= (1 << EXTI8);
		EXTI_CallBackFunc[EXTI8]();
	}

	if (EXTI9_FLAG == Raised){
		/*Clear pending flag, and call IRQ handler*/
		EXTI->PR |= (1 << EXTI9);
		EXTI_CallBackFunc[EXTI9]();
	}
}

void EXTI15_10_IRQHandler (void){   /* EXTI Line[15:10] interrupts */

	if (EXTI10_FLAG == Raised){
		/*Clear pending flag, and call IRQ handler*/
		EXTI->PR |= (1 << EXTI10);
		EXTI_CallBackFunc[EXTI10]();
	}

	if (EXTI11_FLAG == Raised){
		/*Clear pending flag, and call IRQ handler*/
		EXTI->PR |= (1 << EXTI11);
		EXTI_CallBackFunc[EXTI11]();
	}

	if (EXTI12_FLAG == Raised){
		/*Clear pending flag, and call IRQ handler*/
		EXTI->PR |= (1 << EXTI12);
		EXTI_CallBackFunc[EXTI12]();
	}

	if (EXTI13_FLAG == Raised){
		/*Clear pending flag, and call IRQ handler*/
		EXTI->PR |= (1 << EXTI13);
		EXTI_CallBackFunc[EXTI13]();
	}

	if (EXTI14_FLAG == Raised){
		/*Clear pending flag, and call IRQ handler*/
		EXTI->PR |= (1 << EXTI14);
		EXTI_CallBackFunc[EXTI14]();
	}

	if (EXTI15_FLAG == Raised){
		/*Clear pending flag, and call IRQ handler*/
		EXTI->PR |= (1 << EXTI15);
		EXTI_CallBackFunc[EXTI15]();
	}
}
