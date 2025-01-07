/*
 * GPIO_interface.h
 *
 *  Created on: Nov 25, 2024
 *      Author: User
 */

#ifndef GPIO_GPIO_INTERFACE_H_
#define GPIO_GPIO_INTERFACE_H_
#include <stdint.h>
#include <stdio.h>
#include "../../Inc/ErrorTypes.h"
#include "../../Inc/F4_Registers.h"

typedef enum {
	PORTA,
	PORTB,
	PORTC,
	PORTD,
	PORTE,
	PORTF,
	PORTG,
	PORTH
}Port_t;

typedef enum {
	PIN0 ,
	PIN1 ,
	PIN2 ,
	PIN3 ,
	PIN4 ,
	PIN5 ,
	PIN6 ,
	PIN7 ,
	PIN8 ,
	PIN9 ,
	PIN10,
	PIN11,
	PIN12,
	PIN13,
	PIN14,
	PIN15
}Pin_t;

typedef enum {
	AF0 ,
	AF1 ,
	AF2 ,
	AF3 ,
	AF4 ,
	AF5 ,
	AF6 ,
	AF7 ,
	AF8 ,
	AF9 ,
	AF10 ,
	AF11 ,
	AF12 ,
	AF13 ,
	AF14 ,
	AF15
}AltFunc_t;

typedef enum {
	INPUT,
	OUTPUT,
	ALTER_FUNC,
	ANALOG
}Mode_t;

typedef enum {
	PushPull,
	OpenDrain
}OutPut_Type;

typedef enum {
	Low_Speed,
	Medium_Speed,
	Fast_Speed,
	High_Speed
}Out_Speed;

typedef enum {
	NO_PUPD,
	PullUp,
	PullDown
}PullUpDown_t;

typedef enum {
	Low,
	High
}PinValue_t;

typedef struct {
	Port_t Port;
	Pin_t PinNum;
	Mode_t Mode;
	AltFunc_t AltFunc;
	OutPut_Type Out_Type;
	PullUpDown_t PullType;
	Out_Speed  Speed;
}PinConfig_t;

uint8_t GPIO_PinInit(const PinConfig_t* PinConfig);
uint8_t GPIO_SetPinValue(uint32_t port, uint32_t PinNum, uint32_t PinValue);
uint8_t GPIO_ReadPin(uint32_t port, uint32_t PinNum, PinValue_t* PinValue);
uint8_t GPIO_TogglePin(uint32_t port, uint32_t PinNum);




#endif /* GPIO_GPIO_INTERFACE_H_ */
