/*
 * DMA_interface.h
 *
 *  Created on: Jan 8, 2025
 *      Author: User
 */

#ifndef DMA_DMA_INTERFACE_H_
#define DMA_DMA_INTERFACE_H_
#include <stdio.h>
#include <stdint.h>
#include "../../Inc/ErrorTypes.h"
#include "../../Inc/F4_Registers.h"
#include "../../Inc/Bit_Operations.h"

#define Raised         1
#define StreamNum      8
#define CallBackID     5

typedef enum {
	PERIPHERAL_TO_MEMORY,
	MEMORY_TO_PERIPHERAL,
	MEMORY_TO_MEMORY
}Transfer_Direction;

typedef enum {
	DMA_1,
	DMA_2
}DMA_index;

typedef enum {
	Low,
	medium,
	high,
	veryhigh
}Priority_Level;

typedef enum {
	BYTE,
	HALFWORD,
	WORD
}DataSize;

typedef enum {
	DIRECT_MODE,
	FIFO_MODE
}DMA_Transfer_Mode;

typedef enum {
	OneByte,
	HALF_FIFO,
	ThreeBytes,
	FULL_FIFO
}FIFO_threshold;

typedef enum {
	FIFO_ERR_FLAG,
	DirectMode_ERR_FLAG = 2,
   Transfer_ERR_FLAG,
	HALF_Transfer_FLAG,
	Transfer_CMP_FLAG
}DMA_Interrupt_Flag;

typedef enum {
	TRANSFER_CMP_CALLBACK,
	FIFO_ERROR_CALLBACK,
	TRANSFER_ERR_CALLBACK,
	HalfTransfer_ERR_CALLBACK,
	DIRECTMODE_ERR_CALLBACK
}DMA_CallBackID;

typedef struct {
	DMA_index DMAx;
	uint8_t Streamx;
	uint8_t Channel;
	Priority_Level PriorityLevel;
	Transfer_Direction TransferDirection;
	DataSize PeripheralDataSize;
	DataSize MemoryDataSize;
	DMA_Transfer_Mode Transfer_Mode;
	FIFO_threshold FIFOthreshold;
}DMA_Config_t;

uint8_t DMA_Init(const DMA_Config_t* DMA_CONFIG);
uint8_t DMA_StartTransfer(DMA_Config_t* DMA_CONFIG, uint32_t *SrcAddress, uint32_t *DestAddress, uint32_t dataSize);
uint8_t DMA_GetFlagStatus(DMA_index DMAx, uint8_t streamx, DMA_Interrupt_Flag InterruptFlag);
void DMA_ClearInterruptFlag(DMA_index DMAx, uint8_t streamx, DMA_Interrupt_Flag InterruptFlag);
uint8_t DMA_SetCallBack(DMA_Config_t* DMA_CONFIG, DMA_CallBackID CallBack_ID, void (*CallBackFunc)(void));

#endif /* DMA_DMA_INTERFACE_H_ */
