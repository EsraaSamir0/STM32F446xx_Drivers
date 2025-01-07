/*
 * RCC_interface.h
 *
 *  Created on: Nov 26, 2024
 *      Author: User
 */

#ifndef RCC_RCC_INTERFACE_H_
#define RCC_RCC_INTERFACE_H_
#include <stdint.h>
#include <stdio.h>

#define RCC       ((RCC_Reg_t*)RCC_BASE_ADDRESS)

#define TIMEOUT_MAX             500
#define Unused                  0xFF
#define SysClk_MASK             0xFFFFFFFC
#define SysClk_CHECK_MASK       0xC
#define PLL_CFG_MASK            0xFFFC8000
#define AHBprescaler_MASK       0xFFFFFF0F
#define APB1prescaler_MASK      0xFFFFE3FF
#define APB2prescaler_MASK      0xFFFF1FFF


typedef enum {
	HSI,
	HSE,
	PLL_P,
	PLL_R
}SysClk_Source;

typedef enum {
	HSI_ON,
	HSE_ON = 16,
	PLL_ON = 24,
	PLLI2S_ON = 26,
	PLLSAI_ON = 28
}RCC_Clocks_t;

typedef enum {
	PLL_By2,
	PLL_By4,
	PLL_By6,
	PLL_By8
}PLL_P_Factor;

typedef enum {
	OFF,
	ON
}RCC_ClkStatus;

typedef enum {
	SysCLK, SysCLK_by_2= 8, SysCLK_by_4 , SysCLK_by_8, SysCLK_by_16, SysCLK_by_64,
	SysCLK_by_128, SysCLK_by_256, SysCLK_by_512
}AHB_prescaler;

typedef enum {
	AHBCLK, AHBCLK_by2 = 4, AHBCLK_by4, AHBCLK_by8, AHBCLK_by16
}APB_prescaler;

typedef enum {
	HSI_CLK,
	HSE_CLK
}PLL_Source;

typedef enum {
	GPIO_A, GPIO_B, GPIO_C, GPIO_D, GPIO_E, GPIO_F, GPIO_G, GPIO_H, CRC_EN = 12,
	BKPSRAM_EN = 18, DMA1_EN = 21, DMA2_EN, OTGHS_EN = 29, OTGHSULPI_EN
}AHB1peripherals;

typedef enum {
	DCMI_EN, OTGFS_EN = 7
}AHB2peripherals;

typedef enum {
	 FMC_EN, QSPI_EN
}AHB3peripherals;

typedef enum {
	 TIM2_EN, TIM3_EN, TIM4_EN, TIM5_EN,TIM6_EN, TIM7_EN, TIM12_EN, TIM13_EN, TIM14_EN, WWDG_EN= 11,
	 SPI2_EN =14, SPI3_EN, SPDIFRX_EN, USART2_EN, USART3_EN, UART4_EN, UART5_EN, I2C1_EN, I2C2_EN, I2C3_EN,
	 FMPI2C1_EN, CAN1_EN, CAN2_EN, CEC_EN, PWR_EN, DAC_EN
}APB1peripherals;

typedef enum {
	 TIM1_EN, TIM8_EN, USART1_EN = 4, USART6_EN, ADC1_EN = 8, ADC2_EN, ADC3_EN, SDIO_EN, SPI1_EN, SPI4_EN,
	 SYSCFG_EN, TIM9_EN = 16, TIM10_EN, TIM11_EN, SAI1_EN = 22, SAI2_EN
}APB2peripherals;

typedef enum {
	ByPass_OFF,
	ByPass_ON
}HSE_Config_t;

typedef struct {
	PLL_Source PLL_SOURCE;
	uint8_t PLL_M;
	uint8_t PLL_N;
	PLL_P_Factor PLL_P;      // Main PLL divider
	uint8_t PLL_Q;          // USB clock divider
	uint8_t PLL_R;         // I2S clock divider
}PLL_Config_t;


uint8_t RCC_SetClockkStatus(RCC_Clocks_t Clock,RCC_ClkStatus Status);
uint8_t RCC_PLLConfig(const PLL_Config_t* PLL_Config);

void RCC_HSEConfig(HSE_Config_t config);
uint8_t RCC_SetSystemCLK(SysClk_Source Clk_Type);
void RCC_AHB1EnableClk(AHB1peripherals peripheral);
void RCC_AHB1DisableClk(AHB1peripherals peripheral);
void RCC_AHB2EnableClk(AHB2peripherals peripheral);
void RCC_AHB2DisableClk(AHB2peripherals peripheral);
void RCC_AHB3EnableClk(AHB3peripherals peripheral);
void RCC_AHB3DisableClk(AHB3peripherals peripheral);
void RCC_AHB_Prescaler(AHB_prescaler prescaler);
void RCC_APB1EnableClk(APB1peripherals peripheral);
void RCC_APB1DisableClk(APB1peripherals peripheral);
void RCC_APB2EnableClk(APB2peripherals peripheral);
void RCC_APB2DisableClk(APB2peripherals peripheral);



#endif /* RCC_RCC_INTERFACE_H_ */
