/*
 * LED.h
 *
 *  Created on: Dec 7, 2024
 *      Author: User
 */

#ifndef LED_LED_H_
#define LED_LED_H_
#include <stdint.h>
#include <stdio.h>
#include "../../MCAL/GPIO/GPIO_interface.h"

uint8_t LED_Init(const PinConfig_t* LedPin);
uint8_t LED_ON(const PinConfig_t * LedPin);
uint8_t LED_OFF(const PinConfig_t * LedPin);
uint8_t LED_Toggle(const PinConfig_t * LedPin);



#endif /* LED_LED_H_ */
