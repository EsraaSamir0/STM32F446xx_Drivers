/*
 * SCB_program.c
 *
 *  Created on: Dec 1, 2024
 *      Author: User
 */

#include "SCB_interface.h"
#include "../../Inc/ErrorTypes.h"
#include "../../Inc/Bit_Operations.h"
#include "../../Inc/F4_Registers.h"

SCB_Reg_t * SCB = (SCB_Reg_t*)(SCB_BASE_ADDRESS);


uint8_t SCB_SetPRIGROUP(PriorityGroup_t PRIGROUP){

	if (PRIGROUP <= 7) {
		SCB->AIRCR = (0x05FA0000 | (PRIGROUP << 8));
	} else {
		return ERROR;
	}

	return OK;
}
