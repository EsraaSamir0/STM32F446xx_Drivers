/*
 * GPIO_program.c
 *
 *  Created on: Nov 25, 2024
 *      Author: User
 */
#include "GPIO_interface.h"

static GPIO_Reg_t* GPIO[8]= {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH};

uint8_t GPIO_PinInit(const PinConfig_t* PinConfig){

	uint8_t LocalErrorStatus = OK;
	if (PinConfig == NULL){
		return NULL_PTR_ERR;
	}
	if (PinConfig->Port <= PORTH){
		/* Iterate through all pins in PinNum field */
	/*	for (uint16_t pin = 0; pin < 16; pin++) {
			if (PinConfig->PinNum & (1 << pin)) { // Check if pin is set*/

				/*Select Pin Mode*/
				GPIO[PinConfig->Port]->MODER &= ~ (0b11 << PinConfig->PinNum*2);
				GPIO[PinConfig->Port]->MODER |= ((PinConfig->Mode) << PinConfig->PinNum*2);

				/*Select Pull Up & Pull Down*/
				GPIO[PinConfig->Port]->PUPDR &= ~ (0b11 << PinConfig->PinNum*2);
				GPIO[PinConfig->Port]->PUPDR |= ((PinConfig->PullType) << PinConfig->PinNum*2);

				/*Output Pin Settings*/
				if ((PinConfig->Mode == OUTPUT) || (PinConfig->Mode == ALTER_FUNC)){

					/*Set Output Type of Pin*/
					GPIO[PinConfig->Port]->OTYPER &= ~ (1 << PinConfig->PinNum);
					GPIO[PinConfig->Port]->OTYPER |= ((PinConfig->Out_Type) << PinConfig->PinNum);

					/*Set Output Speed of Pin*/
					GPIO[PinConfig->Port]->OSPEEDR &= ~ (0b11 << PinConfig->PinNum*2);
					GPIO[PinConfig->Port]->OSPEEDR |= ((PinConfig->Speed) << PinConfig->PinNum*2);

					/*Set Alternate Function*/
					if (PinConfig->Mode == ALTER_FUNC)
					{
						GPIO[PinConfig->Port]->AFR[(PinConfig->PinNum)/8] &= ~ (0b1111 << ((PinConfig->PinNum) %8)*4);
						GPIO[PinConfig->Port]->AFR[(PinConfig->PinNum)/8] |= ((PinConfig->AltFunc) << ((PinConfig->PinNum) %8)*4);
					}
				}
			}
	else {
		LocalErrorStatus = ERROR;
	}
	return LocalErrorStatus;
}


uint8_t GPIO_SetPinValue(uint32_t port, uint32_t PinNum, uint32_t PinValue){

	uint8_t LocalErrorStatus = OK;
	if (port <= PORTH && PinNum <= PIN15){
		GPIO[port]->ODR &= ~ (1<<PinNum);
		GPIO[port]->ODR |= (PinValue<<PinNum);
	}
	else {
		LocalErrorStatus = ERROR;
	}
	return LocalErrorStatus;
}


uint8_t GPIO_ReadPin(uint32_t port, uint32_t PinNum, PinValue_t *PinValue){

	uint8_t LocalErrorStatus = OK;
	if (port <= PORTH && PinNum <= PIN15){

		*PinValue = ((GPIO[port]->IDR >> PinNum) & 0x01);
	}
	else {

		LocalErrorStatus = ERROR;
	}
	return LocalErrorStatus;
}


uint8_t GPIO_TogglePin(uint32_t port, uint32_t PinNum){

	uint8_t LocalErrorStatus = OK;
	if (port <= PORTH && PinNum <= PIN15){

		GPIO[port]->ODR ^= (1<<PinNum);
	}
	else {

		LocalErrorStatus = ERROR;
	}
	return LocalErrorStatus;

}

