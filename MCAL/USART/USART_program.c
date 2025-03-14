/*
 * USART_program.c
 *
 *  Created on: Dec 4, 2024
 *      Author: User
 */
#include "USART_interface.h"

static USART_Reg_t* USART[6] = {USART1, USART2, USART3, UART4, UART5, USART6};

#define APB1       1
#define APB2       2

/* Must be replaced with the actual HSE, PLL frequencies used in the system */
#define HSE_VALUE  3
#define PLL_VALUE  4

uint32_t Fclk;
static uint32_t GetClockFrequency(uint8_t UART_BUS);
static void USART_ConfigurePins(USART_index usartx);

uint8_t USART_Init(const USART_Config_t* USART_Config){

	if (USART_Config == NULL){
		return NULL_PTR_ERR;
	}
	if ((USART_Config->usartx >= usart1) && (USART_Config->usartx <= usart6) ){

		USART_ConfigurePins(USART_Config->usartx);

		/* Enable Clock for the peripheral */
		switch (USART_Config->usartx){
		case usart1 : RCC_APB2EnableClk(USART1_EN); break;
		case usart2 : RCC_APB1EnableClk(USART2_EN); break;
		case usart3 : RCC_APB1EnableClk(USART3_EN); break;
		case uart4  : RCC_APB1EnableClk(UART4_EN);  break;
		case uart5  : RCC_APB1EnableClk(UART5_EN);  break;
		case usart6 : RCC_APB2EnableClk(USART6_EN); break;
		}

		/*Enable the USART peripheral */
		USART[USART_Config->usartx]->CR1 |= (1 << UE);

		/* Setting USART MODE Tx or Rx or Both */
		USART[USART_Config->usartx]->CR1 &= ~(0b11 << RE);
		USART[USART_Config->usartx]->CR1 |= (USART_Config->Mode << RE);

		/* Setting number of Stop bits */
		USART[USART_Config->usartx]->CR2 &= ~(0b11 << STOP0);
		USART[USART_Config->usartx]->CR2 |= (USART_Config->StopBits << STOP0);

		/* Setting Word length */
		USART[USART_Config->usartx]->CR1 &= ~(0b1 << M);
		USART[USART_Config->usartx]->CR1 |= (USART_Config->WordLength << M);

		/* Checking for Parity enable before configuring it*/
		if (USART_Config->Parity == NO_PARITY){
			USART[USART_Config->usartx]->CR1 &= ~(0b1 << PCE);  /*Disable Parity Control*/
		}else {
			USART[USART_Config->usartx]->CR1 &= ~(0b11 << PS);
			USART[USART_Config->usartx]->CR1 |= (0b1 << PCE);
			USART[USART_Config->usartx]->CR1 |= (USART_Config->Parity << PS);
		}
		/* Setting OverSampling rate */
		USART[USART_Config->usartx]->CR1 &= ~(0b1 << OVER8);
		USART[USART_Config->usartx]->CR1 |= (USART_Config->OverSampling << OVER8);

		/* Setting BaudRate according to PCLK */
		if ((USART_Config->usartx == usart1) || (USART_Config->usartx == usart6)){
			/* APB2 Bus */
			 Fclk = GetClockFrequency(APB2);
		}else { /* APB1 Bus */

			 Fclk = GetClockFrequency(APB1);
		}
		/* BAUDRATE Calculations*/
		float USARTDIV = (float)Fclk / (8 * (2 - USART_Config->OverSampling) * (USART_Config->BaudRate));
		uint32_t DIV = USARTDIV * 1000;
		uint32_t DIV_Matnissa = DIV /1000;
		uint32_t Fraction = DIV % 1000;

		if (USART_Config->OverSampling == By16){
			uint32_t DIV_Fraction = (((Fraction*16)+500)/1000); /*Rounding the fractional part*/
			USART[USART_Config->usartx]->BRR &= ~ (0xFFFF);
			USART[USART_Config->usartx]->BRR = (DIV_Matnissa << 4) | DIV_Fraction;
		}else {
			uint32_t DIV_Fraction = (((Fraction*8)+500)/1000);
			USART[USART_Config->usartx]->BRR &= ~ (0xFFFF);
			USART[USART_Config->usartx]->BRR = (DIV_Matnissa << 4) | DIV_Fraction;
		}
	}
	else {
		return ERROR;
	}
	return OK;
}

uint8_t USART_SendData(USART_index usartx, uint8_t data){

	if (usartx >= usart1 && usartx <= usart6){
		/*wait for transmit data register to be empty*/
		uint32_t timeout = 10000;
		while (GET_BIT(USART[usartx]->SR, TXE) != 1){
			if (--timeout == 0) {
				return TimeoutError;
			}
		}
		USART[usartx]->DR = (uint16_t)data;

		// Wait for TC (Transmission Complete) flag
		while (!(USART[usartx]->SR & (1 << TC)));
		USART[usartx]->SR &= ~ (1 << TC);
	}
	else {
		return ERROR;
	}
	return OK;    // Successful transmission
}

uint8_t USART_SendString(USART_index usartx, const char *str){

	if (str == NULL) {
		return NULL_PTR_ERR;
	}
	if (usartx >= usart1 && usartx <= usart6){
		/* Transmit until null terminator */
		for(uint32_t i = 0 ; str[i] != '\0' ; i++){
			if (USART_SendData(usartx, str[i]) != OK) {
				return ERROR;
			}
		}
	}
	else {
		return ERROR;
	}
	return OK;    // Successful transmission
}

uint8_t USART_Receive(USART_index usartx, uint8_t *UARTdata){

	if (UARTdata == NULL) {
				return NULL_PTR_ERR;
			}
	if (usartx >= usart1 && usartx <= usart6){

		/* wait until received data is ready to be read.*/
		uint32_t timeout = 1000000;
		while (GET_BIT(USART[usartx]->SR, RXNE) != 1){
			timeout --;
			if (timeout == 0) {
				return TimeoutError;
			}
		}
		/* Checking if parity is enabled */
		if (GET_BIT(USART[usartx]->CR1,PCE) ==1){
			*UARTdata = (USART[usartx]->DR & 0xFF);
		}
		else {
			*UARTdata = USART[usartx]->DR;
		}
	}
	else {
		return ERROR;
	}
	return OK;
}

uint8_t USART_SetHardwareFlowControl(USART_index usartx, FLOWCONTROLMODE_t FlowControlMode){

	if (usartx >= usart1 && usartx <= usart6){
	   USART[usartx]->CR3 = (USART[usartx]->CR3 & HW_FLOWCONTROL_MASK) | (FlowControlMode << RTSE);
	}
	else {
		return ERROR;
	}
	return OK;
}

uint8_t USART_DMAEnable(USART_index usartx){

	if (usartx >= usart1 && usartx <= usart6){
		USART[usartx]->CR3 = (USART[usartx]->CR3 & USART_DMA_MASK) | (0b11 << DMAR);
	}
	else {
		return ERROR;
	}
	return OK;
}


static void USART_ConfigurePins(USART_index usartx){

	PinConfig_t USART_config;
	USART_config.Mode = ALTER_FUNC;
	USART_config.Speed = High_Speed;

	switch (usartx){
	case usart1 :
		USART_config.Port = PORTA;
		USART_config.AltFunc = AF7;
		USART_config.PinNum = PIN10;
		USART_config.PullType = PullUp;
		GPIO_PinInit(&USART_config); // Rx
		USART_config.PinNum = PIN9;
		USART_config.Out_Type = PushPull;
		USART_config.PullType = NO_PUPD;
		GPIO_PinInit(&USART_config); // Tx
		break;
	case usart2 :
		USART_config.Port = PORTA;
		USART_config.AltFunc = AF7;
		USART_config.PullType = PullUp;
		USART_config.PinNum = PIN3;
		GPIO_PinInit(&USART_config); // Rx
		USART_config.PinNum = PIN2;
		USART_config.Out_Type = PushPull;
		USART_config.PullType = NO_PUPD;
		GPIO_PinInit(&USART_config); // Tx
		break;
	case usart3 :
		USART_config.Port = PORTB;
		USART_config.AltFunc = AF7;
		USART_config.PinNum = PIN11;
		USART_config.PullType = PullUp;
		GPIO_PinInit(&USART_config); // Rx
		USART_config.PinNum = PIN10;
		USART_config.Out_Type = PushPull;
		USART_config.PullType = NO_PUPD;
		GPIO_PinInit(&USART_config); // Tx
		break;
	case uart4 :
		USART_config.Port = PORTA;
		USART_config.AltFunc = AF8;
		USART_config.PinNum = PIN1;
		USART_config.PullType = PullUp;
		GPIO_PinInit(&USART_config); // Rx
		USART_config.PinNum = PIN0;
		USART_config.Out_Type = PushPull;
		USART_config.PullType = NO_PUPD;
		GPIO_PinInit(&USART_config); // Tx
		break;
	case uart5 :
		USART_config.Port = PORTE;
		USART_config.AltFunc = AF8;
		USART_config.PinNum = PIN7;
		USART_config.PullType = PullUp;
		GPIO_PinInit(&USART_config); // Rx
		USART_config.PinNum = PIN8;
		USART_config.Out_Type = PushPull;
		USART_config.PullType = NO_PUPD;
		GPIO_PinInit(&USART_config); // Tx
		break;
	case usart6 :
		USART_config.Port = PORTC;
		USART_config.AltFunc = AF8;
		USART_config.PinNum = PIN7;
		USART_config.PullType = PullUp;
		GPIO_PinInit(&USART_config); // Rx
		USART_config.PinNum = PIN6;
		USART_config.Out_Type = PushPull;
		USART_config.PullType = NO_PUPD;
		GPIO_PinInit(&USART_config); // Tx
		break;
	}
}


static uint32_t GetClockFrequency(uint8_t UART_BUS){

	const uint32_t AHBPrescTable[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 8, 16, 64, 128, 256, 512 };
	const uint8_t APBPrescTable[8] = { 1, 1, 1, 1, 2, 4, 8, 16};   // 4 states for no division
	uint32_t SysClk_Source = ((RCC->CFGR >> 2) & 0x3);   // Extract bits 3:2 to get the current SysClock source
	uint32_t SysClk_freq = 0;
	switch (SysClk_Source){

	     case 0x0: //HSI
	     	SysClk_freq = 16000000;  // 16 MHz
		    break;
	    case 0x1:  // HSE
	    	SysClk_freq = HSE_VALUE;
		    break;
	    case 0x2:  // PLL
	    	SysClk_freq = PLL_VALUE;
		    break;
	}
	uint32_t ahb_prescaler = ((RCC->CFGR >> HPRE0) & 0xF);
	uint32_t ahb_div = AHBPrescTable[ahb_prescaler];

	if (UART_BUS == APB1){  /* Get clock frequency in case of ABP1 bus*/
		uint32_t apb1_prescaler = ((RCC->CFGR >> PPRE10) & 0x7);
		uint32_t apb1_div = APBPrescTable[apb1_prescaler];
		Fclk = (SysClk_freq / ahb_div / apb1_div);
	}
	else { 	/* APB2 Bus */
		uint32_t apb2_prescaler = ((RCC->CFGR >> PPRE20) & 0x7);
		uint32_t apb2_div = APBPrescTable[apb2_prescaler];
		Fclk = (SysClk_freq / ahb_div / apb2_div);
	}
	return Fclk;
}
