/*
 * RTC_program.c
 *
 *  Created on: Dec 25, 2024
 *      Author: User
 */
#include "DS1307_interface.h"

I2C_Config_t DS1307_RTC = {
		.I2Cx = I2C_1,
		.I2C_Mode = I2C_MASTER,
		.SpeedMode = STANDARD,
		.ClockSpeed = 100000
};

static uint8_t BCDToBinary(uint8_t bcdValue);
static uint8_t BinaryToBCD(uint8_t decimalValue);

void DS1307_Init(void){

	I2C_Init(&DS1307_RTC);

}

uint8_t DS1307_SetTime(uint8_t seconds, uint8_t minutes, uint8_t hours){
	if (seconds > 59 || minutes > 59 || hours > 23){
		return ERROR;
	}

	uint8_t set_time[4];
	set_time[0] = 0x00,  //Set Register Pointer to 0x00
	set_time[1] = BinaryToBCD(seconds) & ~(1 << 7);  // Clear CH bit
	set_time[2] = BinaryToBCD(minutes);
	set_time[3] = BinaryToBCD(hours & 0x3F);  // Force 24-hour mode


	if (I2C_MasterSendData(I2C_1, DS1307_ADDRESS, set_time, 4, RepeatedStart_DISABLE) != OK){
		return ERROR;
	}

	return OK;
}


uint8_t DS1307_SetDate(uint8_t day, uint8_t date, uint8_t month, uint8_t year){
	if (day > 31 || month > 12){
			return ERROR;
		}
	uint8_t set_date[5];
	set_date[0] = 0x03;  // Start at day-of-week register
	set_date[1] = BinaryToBCD(day);
	set_date[2] = BinaryToBCD(date);
	set_date[3] = BinaryToBCD(month);
	set_date[4] = BinaryToBCD(year);

	if (I2C_MasterSendData(I2C_1, DS1307_ADDRESS, set_date, 5, RepeatedStart_DISABLE) != OK){
		return ERROR;
	}

	return OK;
}


uint8_t DS1307_GetTimeAndDate(Time_Date_t *TimeAndDate){

	// Set Register Pointer to 0x00
	uint8_t data = 0x00;
	I2C_MasterSendData(I2C_1, DS1307_ADDRESS, &data, 1, RepeatedStart_DISABLE);

	uint8_t GetTimeAndDate[7];

	if (I2C_MasterReceive(I2C_1, DS1307_ADDRESS, GetTimeAndDate, 7, RepeatedStart_DISABLE) != OK){
		return ERROR;
	}

	TimeAndDate->seconds = BCDToBinary(GetTimeAndDate[0] & 0x7F);  // Mask CH bit
	TimeAndDate->minutes = BCDToBinary(GetTimeAndDate[1]);
	TimeAndDate->hours   = BCDToBinary(GetTimeAndDate[2] & 0x3F);  // Force 24-hr
	TimeAndDate->day = BCDToBinary(GetTimeAndDate[3]);
	TimeAndDate->date = BCDToBinary(GetTimeAndDate[4]);
	TimeAndDate->month = BCDToBinary(GetTimeAndDate[5]);
	TimeAndDate->year = BCDToBinary(GetTimeAndDate[6]);
	return OK;
}


uint8_t BCDToBinary(uint8_t value) {
	uint8_t units = value & 0x0F;
	uint8_t tens = (value & 0xF0)>>4;
    return ((tens*10) + units);
}


uint8_t BinaryToBCD(uint8_t value) {
	uint8_t units = value%10;
	uint8_t tens = value/10;
    return ( (tens << 4) | units);
}

void TimeToStr(uint8_t hours, uint8_t minutes, uint8_t seconds, char *str){
    sprintf(str, "%02d:%02d:%02d\n", hours, minutes, seconds);
}


void DateToStr(uint8_t date, uint8_t month, uint8_t year, char *str){
    sprintf(str, "%02d/%02d/20%02d\n", date, month, year);
}
