/*
 * LED.c
 *
 *  Created on: Dec 7, 2024
 *      Author: User
 */

#include "LED.h"

uint8_t LED_Init(const PinConfig_t* LedPin){

	if (LedPin == NULL){
			return NULL_PTR_ERR;
		}
	GPIO_PinInit(LedPin);
	return OK;
}

uint8_t LED_ON(const PinConfig_t* LedPin){

	if (LedPin == NULL){
		return NULL_PTR_ERR;
	}
	GPIO_SetPinValue(LedPin->Port, LedPin->PinNum, High);
	return OK;
}


uint8_t LED_OFF(const PinConfig_t * LedPin){

	if (LedPin == NULL){
		return NULL_PTR_ERR;
	}
	GPIO_SetPinValue(LedPin->Port, LedPin->PinNum, Low);
	return OK;

}


uint8_t LED_Toggle(const PinConfig_t * LedPin){

	if (LedPin == NULL){
		return NULL_PTR_ERR;
	}
	GPIO_TogglePin(LedPin->Port, LedPin->PinNum);
	return OK;

}
