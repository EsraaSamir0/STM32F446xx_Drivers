/*
 * DMA_program.c
 *
 *  Created on: Jan 8, 2025
 *      Author: User
 */

#include "DMA_interface.h"

static DMA_Reg_t* DMA[2] = { DMA1, DMA2 };

void (*DMA1_CallBackFunc[StreamNum][CallBackID])(void) = {0}; /* Array of pointers to callback functions */
void (*DMA2_CallBackFunc[StreamNum][CallBackID])(void) = {0};


uint8_t DMA_Init(const DMA_Config_t* DMA_CONFIG){

	if (DMA_CONFIG == NULL){
		return NULL_PTR_ERR;
	}
	/* Clear Enable */
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR &= ~(1 << Stream_EN);

	/* Select Channel */
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR &= ~(0b111 << CHSEL);
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR |= (DMA_CONFIG->Channel << CHSEL);

	/* Set Priority Level */
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR &= ~(0b11 << PL);
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR |= (DMA_CONFIG->PriorityLevel << PL);

	/* Set Transfer Direction */
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR &= ~(0b11 << DIR);
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR |= (DMA_CONFIG->TransferDirection << DIR);

	/* Select Circular mode */
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR |= (1 << CIRC);

	/* Set Peripheral Data Size */
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR &= ~(0b11 << PSIZE);
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR |= (DMA_CONFIG->PeripheralDataSize << PSIZE);

	/* Set Memory Data Size */
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR &= ~(0b11 << MSIZE);
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR |= (DMA_CONFIG->MemoryDataSize << MSIZE);

	/* Set Transfer Mode (Direct or FIFO) */
	if (DMA_CONFIG->Transfer_Mode == FIFO_MODE){
		DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SFCR |= (1 << DMDIS);

		/* Set FIFO Threshold */
		DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SFCR &= ~(0b11 << FTH);
		DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SFCR |= (DMA_CONFIG->FIFOthreshold << FTH);

	}else { /* Direct Mode */
		DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SFCR &= ~(1 << DMDIS);
	}

	/* Enable increment for both Memory and Peripheral  */
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR |= (1 << PINC);
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR |= (1 << MINC);

	/* Enable Transfer Complete, Transfer Error and FIFO Error interrupts */
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR |= (1 << DMA_TCIE);
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR |= (1 << DMA_TEIE);
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SFCR |= (1 << FEIE);

	/* Clearing all flags for the stream */
	DMA_ClearInterruptFlag(DMA_CONFIG->DMAx, DMA_CONFIG->Streamx, FIFO_ERR_FLAG);
	DMA_ClearInterruptFlag(DMA_CONFIG->DMAx, DMA_CONFIG->Streamx, DirectMode_ERR_FLAG);
	DMA_ClearInterruptFlag(DMA_CONFIG->DMAx, DMA_CONFIG->Streamx, Transfer_ERR_FLAG);
	DMA_ClearInterruptFlag(DMA_CONFIG->DMAx, DMA_CONFIG->Streamx, HALF_Transfer_FLAG);
	DMA_ClearInterruptFlag(DMA_CONFIG->DMAx, DMA_CONFIG->Streamx, Transfer_CMP_FLAG);

	return OK;
}


uint8_t DMA_StartTransfer(DMA_Config_t* DMA_CONFIG, uint32_t *SrcAddress, uint32_t *DestAddress, uint32_t dataSize){

	if (DMA_CONFIG == NULL){
		return NULL_PTR_ERR;
	}

	/* Set number of data transfers */
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SNDTR = dataSize;

	/* Set Destination & Source address according to data transfer direction */
	if (DMA_CONFIG->TransferDirection == PERIPHERAL_TO_MEMORY || DMA_CONFIG->TransferDirection == MEMORY_TO_MEMORY){
		DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SPAR = (uint32_t)SrcAddress;
		DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SM0AR = (uint32_t)DestAddress;

	}else if (DMA_CONFIG->TransferDirection == MEMORY_TO_PERIPHERAL){
		DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SM0AR = (uint32_t)SrcAddress;
		DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SPAR = (uint32_t)DestAddress;
	}

	/* Enable the DMA stream  */
	DMA[DMA_CONFIG->DMAx]->Stream[DMA_CONFIG->Streamx].SCR |= (1 << Stream_EN);

	return OK;
}


uint8_t DMA_GetFlagStatus(DMA_index DMAx, uint8_t streamx, DMA_Interrupt_Flag InterruptFlag){

	uint8_t extra_gap = 0;
	if (streamx == 2 || streamx == 3 || streamx == 6 || streamx == 7){
		extra_gap = 4;
	}

	uint32_t flag_position = 6 * (streamx%4) + InterruptFlag + extra_gap;
	uint32_t flag_mask = (1 << flag_position);

	uint8_t status = (DMA[DMAx]->ISR[streamx%4] & flag_mask) ? 1 : 0;

	return status;
}

void DMA_ClearInterruptFlag(DMA_index DMAx, uint8_t streamx, DMA_Interrupt_Flag InterruptFlag){

	uint8_t extra_gap = 0;
	if (streamx == 2 || streamx == 3 || streamx == 6 || streamx == 7){
		extra_gap = 4;
	}

	DMA[DMAx]->IFCR[streamx/4] |= (1 << extra_gap <<(streamx%4)*6 << InterruptFlag);

}

uint8_t DMA_SetCallBack(DMA_Config_t* DMA_CONFIG, DMA_CallBackID CallBack_ID, void (*CallBackFunc)(void)){

	if (CallBackFunc == NULL){
		return NULL_PTR_ERR;
	}
	if (CallBack_ID > DIRECTMODE_ERR_CALLBACK || CallBack_ID < TRANSFER_CMP_CALLBACK){
		return ERROR;
	}
	if (DMA_CONFIG->DMAx == DMA_1){
		DMA1_CallBackFunc[DMA_CONFIG->Streamx][CallBack_ID] = CallBackFunc;

	}else if (DMA_CONFIG->DMAx == DMA_2){
		DMA2_CallBackFunc[DMA_CONFIG->Streamx][CallBack_ID] = CallBackFunc;
	}

	return OK;
}


/************************** Handlers Section ***************************/

void DMA1_Stream0_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_1, 0, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 0, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[0][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 0, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 0, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[0][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 0, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 0, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[0][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 0, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 0, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[0][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 0, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 0, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[0][HalfTransfer_ERR_CALLBACK]();
	}
}

void DMA1_Stream1_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_1, 1, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 1, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[1][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 1, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 1, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[1][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 1, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 1, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[1][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 1, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 1, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[1][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 1, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 1, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[1][HalfTransfer_ERR_CALLBACK]();
	}

}
void DMA1_Stream2_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_1, 2, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 2, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[2][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 2, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 2, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[2][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 2, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 2, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[2][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 2, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 2, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[2][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 2, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 2, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[2][HalfTransfer_ERR_CALLBACK]();
	}

}
void DMA1_Stream3_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_1, 3, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 3, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[3][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 3, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 3, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[3][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 3, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 3, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[3][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 3, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 3, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[3][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 3, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 3, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[3][HalfTransfer_ERR_CALLBACK]();
	}

}
void DMA1_Stream4_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_1, 4, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 4, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[4][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 4, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 4, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[4][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 4, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 4, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[4][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 4, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 4, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[4][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 4, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 4, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[4][HalfTransfer_ERR_CALLBACK]();
	}

}
void DMA1_Stream5_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_1, 5, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 5, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[5][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 5, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 5, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[5][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 5, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 5, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[5][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 5, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 5, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[5][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 5, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 5, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[5][HalfTransfer_ERR_CALLBACK]();
	}

}
void DMA1_Stream6_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_1, 6, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 6, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[6][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 6, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 6, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[6][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 6, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 6, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[6][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 6, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 6, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[6][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 6, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 6, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[6][HalfTransfer_ERR_CALLBACK]();
	}

}
void DMA1_Stream7_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_1, 7, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 7, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[7][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 7, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 7, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[7][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 7, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 7, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[7][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 7, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 7, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[7][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_1, 7, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_1, 7, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA1_CallBackFunc[7][HalfTransfer_ERR_CALLBACK]();
	}
}


void DMA2_Stream0_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_2, 0, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 0, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[0][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 0, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 0, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[0][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 0, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 0, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[0][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 0, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 0, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[0][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 0, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 0, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[0][HalfTransfer_ERR_CALLBACK]();
	}

}
void DMA2_Stream1_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_2, 1, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 1, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[1][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 1, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 1, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[1][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 1, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 1, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[1][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 1, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 1, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[1][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 1, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 1, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[1][HalfTransfer_ERR_CALLBACK]();
	}

}
void DMA2_Stream2_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_2, 2, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 2, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[2][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 2, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 2, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[2][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 2, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 2, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[2][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 2, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 2, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[2][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 2, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 2, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[2][HalfTransfer_ERR_CALLBACK]();
	}

}
void DMA2_Stream3_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_2, 3, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 3, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[3][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 3, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 3, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[3][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 3, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 3, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[3][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 3, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 3, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[3][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 3, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 3, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[3][HalfTransfer_ERR_CALLBACK]();
	}

}
void DMA2_Stream4_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_2, 4, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 4, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[4][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 4, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 4, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[4][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 4, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 4, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[4][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 4, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 4, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[4][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 4, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 4, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[4][HalfTransfer_ERR_CALLBACK]();
	}

}
void DMA2_Stream5_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_2, 5, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 5, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[5][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 5, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 5, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[5][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 5, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 5, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[5][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 5, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 5, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[5][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 5, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 5, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[5][HalfTransfer_ERR_CALLBACK]();
	}

}
void DMA2_Stream6_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_2, 6, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 6, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[6][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 6, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 6, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[6][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 6, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 6, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[6][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 6, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 6, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[6][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 6, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 6, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[6][HalfTransfer_ERR_CALLBACK]();
	}

}
void DMA2_Stream7_IRQHandler (void){
	/* Check which interrupt flag is raised */

	if (DMA_GetFlagStatus(DMA_2, 7, FIFO_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 7, FIFO_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[7][FIFO_ERROR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 7, Transfer_CMP_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 7, Transfer_CMP_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[7][TRANSFER_CMP_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 7, Transfer_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 7, Transfer_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[7][TRANSFER_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 7, DirectMode_ERR_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 7, DirectMode_ERR_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[7][DIRECTMODE_ERR_CALLBACK]();
	}

	if (DMA_GetFlagStatus(DMA_2, 7, HALF_Transfer_FLAG) == Raised){
		/* Clear flag */
		DMA_ClearInterruptFlag(DMA_2, 7, HALF_Transfer_FLAG);
		/* Call the Callback function */
		DMA2_CallBackFunc[7][HalfTransfer_ERR_CALLBACK]();
	}
}
