/*
 * USART_interface.h
 *
 *  Created on: Dec 4, 2024
 *      Author: User
 */

#ifndef USART_USART_INTERFACE_H_
#define USART_USART_INTERFACE_H_
#include <stdio.h>
#include <stdint.h>
#include "../../Inc/ErrorTypes.h"
#include "../../Inc/F4_Registers.h"
#include "../../Inc/Bit_Operations.h"
#include "../RCC/RCC_interface.h"
#include "../GPIO/GPIO_interface.h"

#define HW_FLOWCONTROL_MASK     0xFFFFFCFF
#define USART_DMA_MASK          0xFFFFFF3F

typedef enum {
	usart1,
	usart2,
	usart3,
	uart4,
	uart5,
	usart6
}USART_index;

typedef enum {
	RX_ONLY = 1,
	TX_ONLY,
	RX_TX
}UART_Mode_t;

typedef enum {
	ONE,
	HALF,
	TWO,
	ONE_FIFTY
}STOP_Bits_t;

typedef enum {
	EIGHT,
	NINE
}WORD_Lenght_t;

typedef enum {
	EVEN,
	ODD,
	NO_PARITY
}Parity_t;

typedef enum {
	By16,
	By8
}OverSampling_t;

typedef enum {
	RTS_ONLY =1,
	CTS_ONLY,
	RTS_CTS
}FLOWCONTROLMODE_t;

typedef struct {
	USART_index usartx;
	UART_Mode_t Mode;
	uint32_t BaudRate;
	WORD_Lenght_t WordLength;
	STOP_Bits_t StopBits;
	Parity_t Parity;
	OverSampling_t OverSampling;
}USART_Config_t;

uint8_t USART_Init(const USART_Config_t* USART_Config);
uint8_t USART_SendData(USART_index usartx, uint8_t data);
uint8_t USART_SendString(USART_index usartx, const char *str);
uint8_t USART_Receive(USART_index usartx, uint8_t *UARTdata);
uint8_t USART_SetHardwareFlowControl(USART_index usartx, FLOWCONTROLMODE_t FlowControlMode);
uint8_t USART_DMAEnable(USART_index usartx);




#endif /* USART_USART_INTERFACE_H_ */
