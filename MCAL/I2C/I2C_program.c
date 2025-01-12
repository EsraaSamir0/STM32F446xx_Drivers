/*
 * I2C_program.c
 *
 *  Created on: Dec 20, 2024
 *      Author: User
 */
#include "I2C_interface.h"

static I2C_Reg_t* I2C[3] = {I2C1, I2C2, I2C3};

static void I2C_ConfigurePins(I2C_index I2Cx);

uint8_t I2C_Init(const I2C_Config_t* I2C_Config){

	if (I2C_Config == NULL){
		return NULL_PTR_ERR;
	}
	if (I2C_Config->I2Cx < I2C_1 || I2C_Config->I2Cx > I2C_3){
		return ERROR;
	}

	I2C_ConfigurePins(I2C_Config->I2Cx);

	/* Enable Clock for the peripheral */
	switch (I2C_Config->I2Cx){
	case I2C_1 : RCC_APB1EnableClk(I2C1_EN); break;
	case I2C_2 : RCC_APB1EnableClk(I2C2_EN); break;
	case I2C_3 : RCC_APB1EnableClk(I2C3_EN); break;
	}

	/* Ensure the peripheral is disabled before configuration  */
	I2C[I2C_Config->I2Cx]->CR1 &= ~(1 << I2C_PE);

	/* Set peripheral clock frequency to 16Mhz */
	I2C[I2C_Config->I2Cx]->CR2 = (I2C[I2C_Config->I2Cx]->CR2 & I2C_FREQ_MASK) | (I2C_FREQ << FREQ0) ;

	/* Clock Configuration (Master Mode) */
	uint16_t CCR = 0, TRISE = 0;
	switch (I2C_Config->SpeedMode){

	case STANDARD :
		I2C[I2C_Config->I2Cx]->CCR &= ~(1 << F_S);   // Clear F/S for standard mode
		CCR = (Pclk1_Freq / (2 * I2C_Config->ClockSpeed));
		TRISE = (I2C_FREQ + 1);        // Max rise time: 1000 ns
		break;

	case FAST_Duty0 :
		I2C[I2C_Config->I2Cx]->CCR |= (1 << F_S);      // Set F/S for Fast mode
		I2C[I2C_Config->I2Cx]->CCR &= ~(1 << DUTY);
		CCR = (Pclk1_Freq / (3 * I2C_Config->ClockSpeed));
		TRISE = ((Pclk1_Freq / 3333333) + 1);          // Max rise time: 300 ns
		break;

	case FAST_Duty1 :
		I2C[I2C_Config->I2Cx]->CCR |= (1 << F_S);      // Set F/S for Fast mode
		I2C[I2C_Config->I2Cx]->CCR |= (1 << DUTY);
		CCR = (Pclk1_Freq / (25 * I2C_Config->ClockSpeed));
		TRISE = ((Pclk1_Freq / 3333333) + 1);          // Max rise time: 300 ns
		break;

	default :   /* Standard mode as default mode */
		I2C[I2C_Config->I2Cx]->CCR &= ~(1 << F_S);
		CCR = (Pclk1_Freq / (2 * I2C_Config->ClockSpeed));
		TRISE = (I2C_FREQ + 1);
		break;
	}

	I2C[I2C_Config->I2Cx]->CCR |= (CCR << CCR0);
	I2C[I2C_Config->I2Cx]->TRISE = (TRISE & TRISE_MASK);


	/* Slave Mode Configuration */
	if (I2C_Config->I2C_Mode == I2C_SLAVE){

		/* Enable clock stretching */
		I2C[I2C_Config->I2Cx]->CR1 &= ~(1 << NOSTRETCH);
		I2C[I2C_Config->I2Cx]->OAR1 = (I2C[I2C_Config->I2Cx]->OAR1 & OWN_ADDRESS_MASK);  // Clear old address

		/* Setting the Addressing Mode */
		I2C[I2C_Config->I2Cx]->OAR1 |= (I2C_Config->AddressingMode << ADDMODE);

		/* Setting Own Address in case of SLAVE mode */
		if (I2C_Config->AddressingMode == SevenBit){
			I2C[I2C_Config->I2Cx]->OAR1 |= (I2C_Config->OwnAddress << I2C_ADD1);
		}
		else {
			I2C[I2C_Config->I2Cx]->OAR1 |= (I2C_Config->OwnAddress << I2C_ADD0);
		}
	}
	/* Enabling I2C Peripheral */
	I2C[I2C_Config->I2Cx]->CR1 |= (1 << I2C_PE);

	/* Acknowledge enable */
	I2C[I2C_Config->I2Cx]->CR1 |= (1 << ACK);

	return OK;
}


uint8_t I2C_MasterSendData(I2C_index I2Cx, uint8_t Address, uint8_t *data, uint16_t size, uint8_t repeatedStart){

	if (I2Cx < I2C_1 || I2Cx > I2C_3 || size == 0) {
		return ERROR;
	}
	if (data == NULL){
		return NULL_PTR_ERR;
	}
	uint32_t Timeout = I2C_TIMEOUT;

	/* Generate START condition & Wait for the SB flag to be set (Start condition sent) */
	I2C[I2Cx]->CR1 |= (1 << I2C_START);
	while (GET_BIT(I2C[I2Cx]->SR1,SB) == 0){
		if (--Timeout == 0) {
			return TimeoutError;
		}
	}

	/* Send the slave address with write bit (LSB = 0) */
	I2C[I2Cx]->DR = (Address << 1) | 0x00;

	/* Wait for slave address to be acknowledged */
	while (GET_BIT(I2C[I2Cx]->SR1, ADDR) == 0) {
		if (--Timeout == 0) {
			return TimeoutError;
		}
	}
	/* Clear the ADDR flag */
	volatile uint32_t temp = I2C[I2Cx]->SR1;
	temp = I2C[I2Cx]->SR2;
	(void)temp;

	for (uint16_t i = 0; i < size; i++) {
		/* Wait for Data register to be empty */
		Timeout = I2C_TIMEOUT;
		while (GET_BIT(I2C[I2Cx]->SR1, I2C_TxE) == 0) {
			if (--Timeout == 0) {
				return TimeoutError;
			}
		}
		I2C[I2Cx]->DR = data[i];

		/* Wait for TxE flag */
		Timeout = I2C_TIMEOUT;
		while (GET_BIT(I2C[I2Cx]->SR1, I2C_TxE) == 0) {
			if (--Timeout == 0) {
				return TimeoutError;
			}
		}
	}
	/* Wait for the last byte to be transferred */
	Timeout = I2C_TIMEOUT;
	while (GET_BIT(I2C[I2Cx]->SR1, BTF) == 0) {
		if (--Timeout == 0) {
			return TimeoutError;
		}
	}

	/* Send Stop bit */
	if(repeatedStart == RepeatedStart_DISABLE){
		I2C[I2Cx]->CR1 |= (1 << I2C_STOP);
	}

	return OK;   //Transfer is complete
}

uint8_t I2C_MasterReceive(I2C_index I2Cx, uint8_t Address, uint8_t *data, uint16_t size, uint8_t repeatedStart){

	if (I2Cx < I2C_1 || I2Cx > I2C_3 || size == 0) {
		return ERROR;
	}
	if (data == NULL){
		return NULL_PTR_ERR;
	}
	uint32_t Timeout = I2C_TIMEOUT;

	/* Generate START condition & Wait for the SB flag to be set (Start condition sent) */
	I2C[I2Cx]->CR1 |= (1 << I2C_START);
	while (GET_BIT(I2C[I2Cx]->SR1,SB) == 0){
		if (--Timeout == 0) {
			return TimeoutError;
		}
	}

	/* Send the slave address with Read bit (LSB = 1) */
	I2C[I2Cx]->DR = (Address << 1) | 1;

	/* Wait for slave address to be acknowledged */
	while (GET_BIT(I2C[I2Cx]->SR1, ADDR) == 0) {
		if (--Timeout == 0) {
			return TimeoutError;
		}
	}
	/* Clear the ADDR flag */
	volatile uint32_t temp = I2C[I2Cx]->SR1;
	temp = I2C[I2Cx]->SR2;
	(void)temp;

	for (uint16_t i = 0; i < size; i++) {
		if (i == size - 1) {
			/* Disable ACK for the last byte */
			I2C[I2Cx]->CR1 &= ~(1 << ACK);
		}
		/* Wait until received data is ready to be read.*/
		Timeout = I2C_TIMEOUT;
		while (GET_BIT(I2C[I2Cx]->SR1, I2C_RxNE) == 0) {
			if (--Timeout == 0) {
				return TimeoutError;
			}
		}
		if((i==size-1) && (repeatedStart == RepeatedStart_DISABLE)){
				I2C[I2Cx]->CR1 |= (1 << I2C_STOP);
			}
		data[i] = I2C[I2Cx]->DR;
	}

	return OK;
}


uint8_t I2C_SlaveSendData(I2C_index I2Cx, uint8_t data){

	if (I2Cx < I2C_1 || I2Cx > I2C_3) {
			return ERROR;
		}
	I2C[I2Cx]->DR = data;
	return OK;
}

uint8_t I2C_SlaveReceiveData(I2C_index I2Cx, uint8_t *data){

	if (I2Cx < I2C_1 || I2Cx > I2C_3) {
			return ERROR;
		}
	if (data == NULL){
		return NULL_PTR_ERR;
	}
	*data = I2C[I2Cx]->DR;
	return OK;
}


uint8_t I2C_DMAEnable(I2C_index I2Cx){

	if (I2Cx > I2C_3 || I2Cx < I2C_1){
		return ERROR;
	}
	I2C[I2Cx]->CR2 |= (1 << DMAEN);

	return OK;
}


I2C_Errors I2C_CheckError(I2C_index I2Cx){

	if (I2Cx > I2C_3 || I2Cx < I2C_1){
		return ERROR;
	}
	if (GET_BIT(I2C[I2Cx]->SR1, BERR) == 1){
		I2C[I2Cx]->SR1 &= ~(1 << BERR); // Clear BERR
		return BUS_ERROR;
	}
	if (GET_BIT(I2C[I2Cx]->SR1, ARLO) == 1){
		I2C[I2Cx]->SR1 &= ~(1 << ARLO); // Clear ARLO
		return ARBITRATION_LOST_ERROR;
	}
	if (GET_BIT(I2C[I2Cx]->SR1, I2C_OVR) == 1){
		I2C[I2Cx]->SR1 &= ~(1 << I2C_OVR); // Clear OVR
		return OVERRUN_ERROR;
	}
	if (GET_BIT(I2C[I2Cx]->SR1, AF) == 1){
		I2C[I2Cx]->SR1 &= ~(1 << AF); // Clear AF
		return ACK_FAILURE_ERROR;
	}
	if (GET_BIT(I2C[I2Cx]->SR1, TIMEOUT) == 1){
		I2C[I2Cx]->SR1 &= ~(1 << TIMEOUT); // Clear TIMEOUT
		return TimeoutError;
	}
	return OK; // No error detected
}

void I2C_Reset(I2C_index I2Cx){

	I2C[I2Cx]->CR1 |= (1<<SWRST);   // Software reset
	SysTick_delay_us(1);
	I2C[I2Cx]->CR1 &= ~(1<<SWRST); // Exit reset
}

static void I2C_ConfigurePins(I2C_index I2Cx){
	PinConfig_t I2C_config;
	I2C_config.Mode = ALTER_FUNC;
	I2C_config.AltFunc = AF4;
	I2C_config.Out_Type = OpenDrain;
	I2C_config.PullType = PullUp;
	I2C_config.Speed = High_Speed;

	switch (I2Cx){
	case I2C_1 :
		I2C_config.Port = PORTB;
		I2C_config.PinNum = PIN6;
		GPIO_PinInit(&I2C_config); // SCL
		I2C_config.Port = PORTB;
		I2C_config.PinNum = PIN7;
		GPIO_PinInit(&I2C_config); // SDA
		break;
	case I2C_2 :
		I2C_config.Port = PORTB;
		I2C_config.PinNum = PIN10;
		GPIO_PinInit(&I2C_config); // SCL
		I2C_config.Port = PORTB;
		I2C_config.PinNum = PIN3;
		GPIO_PinInit(&I2C_config); // SDA
		break;
	case I2C_3 :
		I2C_config.Port = PORTA;
		I2C_config.PinNum = PIN8;
		GPIO_PinInit(&I2C_config); // SCL
		I2C_config.Port = PORTB;
		I2C_config.PinNum = PIN4;
		GPIO_PinInit(&I2C_config); // SDA
	}

}
