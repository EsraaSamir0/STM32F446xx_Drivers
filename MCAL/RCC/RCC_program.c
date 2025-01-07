/*
 * RCC_program.c
 *
 *  Created on: Nov 26, 2024
 *      Author: User
 */
#include "RCC_interface.h"
#include "../../Inc/ErrorTypes.h"
#include "../../Inc/Bit_Operations.h"
#include "../../Inc/F4_Registers.h"



uint8_t RCC_SetClkStatus(RCC_Clocks_t Clock,RCC_ClkStatus Status){

	RCC->CR &= ~ (1 << Clock);
	RCC->CR |=  (Status << Clock);
	if (Status == ON){
		uint8_t timeout = 0;
		/* Wait for clock to be ready */
		while (GET_BIT(RCC->CR,(Clock+1)) != 1){
			timeout++ ;
			if(timeout > TIMEOUT_MAX){
				return TimeoutError;
			}
		}
	}
	return OK;
}

uint8_t RCC_PLLConfig(const PLL_Config_t* PLL_Config){

	if (PLL_Config == NULL){
		return NULL_PTR_ERR;
	}
	/* Setting PLL Clock Source */
	RCC->PLLCFGR &= ~(1 << PLLSRC);
	RCC->PLLCFGR |= (PLL_Config->PLL_SOURCE << PLLSRC);

	/* Checking for valid values for PLL_M, PLL_N */
	if ((PLL_Config->PLL_M < 2 || PLL_Config->PLL_M > 63) || (PLL_Config->PLL_N < 50 || PLL_Config->PLL_N > 432)){
		return OUT_OF_RANGE;
	}
	/* Setting PLL_M and PLL_N */
	RCC->PLLCFGR = (RCC->PLLCFGR & PLL_CFG_MASK) | (PLL_Config->PLL_M << PLLM0)| (PLL_Config->PLL_N << PLLN0);

	/* Set PLL_P if provided */
	if (PLL_Config->PLL_P != Unused){
		RCC->PLLCFGR |= (PLL_Config->PLL_P << PLLP0) ;
	}
	/* Set PLL_Q if provided and within range */
	if ((PLL_Config->PLL_Q != Unused) &&(PLL_Config->PLL_Q >= 2) && (PLL_Config->PLL_Q <= 15)){
		RCC->PLLCFGR |= (PLL_Config->PLL_Q << PLLQ0) ;
	}
	/* Set PLL_R if provided and within range */
	if ((PLL_Config->PLL_R != Unused) && (PLL_Config->PLL_R >= 2) && (PLL_Config->PLL_R <= 7)){
		RCC->PLLCFGR |= (PLL_Config->PLL_R << PLLR0) ;
	}
	return OK;
}


void RCC_HSEConfig(HSE_Config_t config){

	if (config == ByPass_OFF){

		CLR_BIT(RCC->CR, HSEBYP);

	}else if (config == ByPass_ON){

		SET_BIT(RCC->CR, HSEBYP);
	}
}

uint8_t RCC_SetSystemCLK(SysClk_Source Clk_Type){

	RCC->CFGR = (RCC->CFGR & SysClk_MASK) | (Clk_Type << SW0);
	RCC->CR |= (1 << CSSON);     /* Enable Clock Security System (CSS) */
	if ((RCC->CFGR & SysClk_CHECK_MASK) != Clk_Type){
		return ERROR;
	}
	return OK;
}

void RCC_AHB_Prescaler(AHB_prescaler prescaler){
	  RCC->CFGR = (RCC->CFGR & AHBprescaler_MASK) | (prescaler << HPRE0);
}

void RCC_APB1_Prescaler(APB_prescaler prescaler){
	  RCC->CFGR = (RCC->CFGR & APB1prescaler_MASK) | (prescaler << PPRE10);
}

void RCC_APB2_Prescaler(APB_prescaler prescaler){
	  RCC->CFGR = (RCC->CFGR & APB2prescaler_MASK) | (prescaler << PPRE20);
}

void RCC_AHB1EnableClk(AHB1peripherals peripheral){
	SET_BIT(RCC->AHB1ENR,peripheral);
}

void RCC_AHB1DisableClk(AHB1peripherals peripheral){
	CLR_BIT(RCC->AHB1ENR,peripheral);
}

void RCC_AHB2EnableClk(AHB2peripherals peripheral){
	SET_BIT(RCC->AHB2ENR,peripheral);
}

void RCC_AHB2DisableClk(AHB2peripherals peripheral){
	CLR_BIT(RCC->AHB2ENR,peripheral);
}

void RCC_AHB3EnableClk(AHB3peripherals peripheral){
	SET_BIT(RCC->AHB3ENR,peripheral);
}

void RCC_AHB3DisableClk(AHB3peripherals peripheral){
	CLR_BIT(RCC->AHB3ENR,peripheral);
}

void RCC_APB1EnableClk(APB1peripherals peripheral){
	SET_BIT(RCC->APB1ENR,peripheral);
}

void RCC_APB1DisableClk(APB1peripherals peripheral){

	CLR_BIT(RCC->APB1ENR,peripheral);
}

void RCC_APB2EnableClk(APB2peripherals peripheral){

	SET_BIT(RCC->APB2ENR,peripheral);
}

void RCC_APB2DisableClk(APB2peripherals peripheral){

	CLR_BIT(RCC->APB2ENR,peripheral);
}
