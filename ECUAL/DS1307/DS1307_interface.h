/*
 * RTC_interface.h
 *
 *  Created on: Dec 25, 2024
 *      Author: User
 */

#ifndef DS1307_DS1307_INTERFACE_H_
#define DS1307_DS1307_INTERFACE_H_
#include <stdint.h>
#include <stdio.h>
#include "../../MCAL/GPIO/GPIO_interface.h"
#include "../../MCAL/I2C/I2C_interface.h"
#include "../../MCAL/SysTick/SysTick_interface.h"

#define DS1307_ADDRESS       0x68


typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;   //day of week
	uint8_t date;
	uint8_t month;
	uint8_t year;
}Time_Date_t;

void DS1307_Init(void);
uint8_t DS1307_SetTime(uint8_t seconds, uint8_t minutes, uint8_t hours);
uint8_t DS1307_SetDate(uint8_t day, uint8_t date, uint8_t month, uint8_t year);
uint8_t DS1307_GetTimeAndDate(Time_Date_t *TimeAndDate);

void TimeToStr(uint8_t hours, uint8_t minutes, uint8_t seconds, char *str);
void DateToStr(uint8_t date, uint8_t month, uint8_t year, char *str);



#endif /* DS1307_DS1307_INTERFACE_H_ */
