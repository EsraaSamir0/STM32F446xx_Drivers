/*
 * SYSCFG_program.c
 *
 *  Created on: Dec 11, 2024
 *      Author: User
 */
#include "SYSCFG_interface.h"


uint8_t SYSCFG_SetEXTIPort(EXTI_Line_t EXTI_LINE, Port_t Port){

	if (EXTI_LINE <= EXTI15 && Port <= PORTH)
	{
		uint8_t RegNum = (EXTI_LINE / 4);
		uint8_t BitNum = ((EXTI_LINE % 4)*4);
		SYSCFG->EXTICR[RegNum] |= (Port << BitNum);
	}
	else {
		return ERROR;
	}
	return OK;
}

