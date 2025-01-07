/*
 * SysTick_program.c
 *
 *  Created on: Nov 16, 2024
 *      Author: User
 */
#include "../../MCAL/SysTick/SysTick_interface.h"

SysTick_Reg_t *SysTick = (SysTick_Reg_t *)(SYSTICK_BASE_ADDRESS);

/* Using CLK = AHB/8 ---> SysTick Frequency = 1 MHz*/
#define TICKS_Per_ms    1000
#define TICKS_Per_us    1

void SysTick_delay_ms(float time){

	CLR_BIT(SysTick->CSR, CLKSOURCE);        /* CLK source (AHB/8) */
	uint32_t  Ticks = (TICKS_Per_ms*time);
	SysTick->RVR = Ticks;
	SysTick->CVR = 0;
	SET_BIT(SysTick->CSR ,ENABLE);                 /* Enable SysTick timer */
	while ((GET_BIT(SysTick->CSR,COUNTFLAG)) == 0);     /*busy wait until the flag is set*/
	CLR_BIT(SysTick->CSR ,ENABLE);

}

void SysTick_delay_us(float time){

	CLR_BIT(SysTick->CSR, CLKSOURCE);        /* CLK source (AHB/8) */
	uint32_t  Ticks = (TICKS_Per_us*time);
	SysTick->RVR = Ticks;
	SysTick->CVR = 0;
	SET_BIT(SysTick->CSR ,ENABLE);           /* Enable SysTick timer */
	while ((GET_BIT(SysTick->CSR,COUNTFLAG)) == 0);     /*busy wait until the flag is set*/
	CLR_BIT(SysTick->CSR ,ENABLE);
}
