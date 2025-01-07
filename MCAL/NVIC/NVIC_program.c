/*
 * NVIC_program.c
 *
 *  Created on: Nov 27, 2024
 *      Author: User
 */


#include "NVIC_interface.h"
#include "../../Inc/ErrorTypes.h"
#include "../../Inc/F4_Registers.h"

#define NVIC_MAX_IRQ  96

NVIC_Reg_t  *NVIC = (NVIC_Reg_t*)(NVIC_BASE_ADDRESS) ;

uint8_t NVIC_EnableInterrupt(NVIC_IRQ_t IRQNum){
	uint8_t Local_Errorstatus = OK ;
	if (IRQNum >= 0 && IRQNum <= NVIC_MAX_IRQ){
		uint8_t RegNum = IRQNum / 32;
		uint8_t BitNum = IRQNum % 32;
		NVIC->ISER[RegNum] = (1 << BitNum);
	}
	else {
		Local_Errorstatus = ERROR;
	}
	return Local_Errorstatus;
}

uint8_t NVIC_DisableInterrupt(NVIC_IRQ_t IRQNum){
	uint8_t Local_Errorstatus = OK ;
	if (IRQNum >= 0 && IRQNum <= NVIC_MAX_IRQ){
		uint8_t RegNum = IRQNum / 32;
		uint8_t BitNum = IRQNum % 32;
		NVIC->ICER[RegNum] = (1 << BitNum);
	}
	else {
		Local_Errorstatus = ERROR;
	}
	return Local_Errorstatus;
}

uint8_t NVIC_SetPendingFlag(NVIC_IRQ_t IRQNum){
	uint8_t Local_Errorstatus = OK ;
	if (IRQNum >= 0 && IRQNum <= NVIC_MAX_IRQ){
		uint8_t RegNum = IRQNum / 32;
		uint8_t BitNum = IRQNum % 32;
		NVIC->ISPR[RegNum] = (1 << BitNum);
	}
	else {
		Local_Errorstatus = ERROR;
	}

	return Local_Errorstatus;
}

uint8_t NVIC_ClearPendingFlag(NVIC_IRQ_t IRQNum){
	uint8_t Local_Errorstatus = OK ;
	if (IRQNum >= 0 && IRQNum <= NVIC_MAX_IRQ){
		uint8_t RegNum = IRQNum / 32;
		uint8_t BitNum = IRQNum % 32;
		NVIC->ICPR[RegNum] = (1 << BitNum);
	}
	else {
		Local_Errorstatus = ERROR;
	}

	return Local_Errorstatus;
}

uint8_t NVIC_GetActiveFlag(NVIC_IRQ_t IRQNum, ActiveFlagStatus_t* status){
	uint8_t Local_Errorstatus = OK ;

	if (status == NULL){
		return NULL_PTR_ERR;
	}
	else {
		if (IRQNum >= 0 && IRQNum <= NVIC_MAX_IRQ){
			uint8_t RegNum = IRQNum / 32;
			uint8_t BitNum = IRQNum % 32;
			*status = (ActiveFlagStatus_t)((NVIC->IABR[RegNum] >> BitNum) & 0x01);
		}
		else {
			Local_Errorstatus = ERROR;  // Invalid IRQ number
		}
	}
	return Local_Errorstatus;
}

uint8_t NVIC_SetIRQPriority(NVIC_IRQ_t IRQNum, uint32_t priority){
	uint8_t Local_Errorstatus = OK ;

	if (IRQNum >= 0 && IRQNum <= NVIC_MAX_IRQ && priority <= 0xF){
		NVIC->IPR[IRQNum] &= ~ (0xF << 4);
		// Setting the priority in the upper 4 bits
		NVIC->IPR[IRQNum] |= (priority << 4);
	}
	else {
		Local_Errorstatus = ERROR;
	}
	return Local_Errorstatus;
}

