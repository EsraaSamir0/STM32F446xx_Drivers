/*
 * I2C_interface.h
 *
 *  Created on: Dec 20, 2024
 *      Author: User
 */

#ifndef I2C_I2C_INTERFACE_H_
#define I2C_I2C_INTERFACE_H_
#include <stdint.h>
#include <stdio.h>
#include "../../Inc/ErrorTypes.h"
#include "../../Inc/F4_Registers.h"
#include "../../Inc/Bit_Operations.h"
#include "../RCC/RCC_interface.h"
#include "../GPIO/GPIO_interface.h"

#define OWN_ADDRESS_MASK       0xFFFFFC00
#define I2C_FREQ_MASK          0xFFFFFFC0
#define TRISE_MASK             0x3F
#define Pclk1_Freq             16000000
#define I2C_FREQ	           16
#define I2C_TIMEOUT            10000

#define RepeatedStart_DISABLE   0
#define RepeatedStart_ENABLE    1

typedef enum {
	I2C_1,
	I2C_2,
	I2C_3
}I2C_index;

typedef enum {
	I2C_SLAVE,
	I2C_MASTER
}I2C_Mode_t;

typedef enum {
	STANDARD,
	FAST_Duty0,
	FAST_Duty1
}SpeedMode_t;

typedef enum {
	SevenBit,
	TenBit
}Addressing_Mode;

typedef enum {
	ACK_FAILURE_ERROR,
	OVERRUN_ERROR,
	BUS_ERROR,
	ARBITRATION_LOST_ERROR
}I2C_Errors;

typedef struct {
	I2C_index I2Cx;
	I2C_Mode_t I2C_Mode;
	SpeedMode_t SpeedMode;
	uint32_t ClockSpeed;
	Addressing_Mode AddressingMode;
	uint8_t OwnAddress;
}I2C_Config_t;

uint8_t I2C_Init(const I2C_Config_t* I2C_Config);
uint8_t I2C_DMAEnable(I2C_index I2Cx);
I2C_Errors I2C_CheckError(I2C_index I2Cx);
void I2C_Reset(I2C_index I2Cx);

uint8_t I2C_MasterSendData(I2C_index I2Cx, uint8_t Address, uint8_t *data, uint16_t size, uint8_t repeatedStart);
uint8_t I2C_MasterReceive(I2C_index I2Cx, uint8_t Address, uint8_t *data, uint16_t size, uint8_t repeatedStart);

uint8_t I2C_SlaveSendData(I2C_index I2Cx, uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_index I2Cx, uint8_t *data);

#endif /* I2C_I2C_INTERFACE_H_ */
