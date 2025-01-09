/*
 * Registers.h
 *
 *  Created on: Nov 24, 2024
 *      Author: User
 */

#ifndef F4_REGISTERS_H_
#define F4_REGISTERS_H_

#include <stdint.h>

/***************************   Memories Base Addresses    ***********************/



/***************************   CORE Peripheral Base Addresses    *********************/

#define SCB_BASE_ADDRESS        0xE000E008U
#define SYSTICK_BASE_ADDRESS    0xE000E010U
#define NVIC_BASE_ADDRESS       0xE000E100U
#define MPU_BASE_ADDRESS        0xE000ED90U
#define FPU_BASE_ADDRESS        0xE000EF30U

/***************************   APB1 Base Addresses    ******************************/

#define TIM2_BASE_ADDRESS                0x40000000U
#define TIM3_BASE_ADDRESS                0x40000400U
#define TIM4_BASE_ADDRESS                0x40000800U
#define TIM5_BASE_ADDRESS                0x40000C00U
#define TIM6_BASE_ADDRESS                0x40001000U
#define TIM7_BASE_ADDRESS                0x40001400U
#define TIM12_BASE_ADDRESS               0x40001800U
#define TIM13_BASE_ADDRESS               0x40001C00U
#define TIM14_BASE_ADDRESS               0x40002000U
#define WWDG_BASE_ADDRESS                0x40002C00U
#define IWDG_BASE_ADDRESS                0x40003000U
#define SPI2_BASE_ADDRESS                0x40003800U
#define SPI3_BASE_ADDRESS                0x40003C00U
#define USART2_BASE_ADDRESS              0x40004400U
#define USART3_BASE_ADDRESS              0x40004800U
#define UART4_BASE_ADDRESS               0x40004C00U
#define UART5_BASE_ADDRESS               0x40005000U
#define I2C1_BASE_ADDRESS                0x40005400U
#define I2C2_BASE_ADDRESS                0x40005800U
#define I2C3_BASE_ADDRESS                0x40005C00U
#define CAN1_BASE_ADDRESS                0x40006400U
#define CAN2_BASE_ADDRESS                0x40006800U
#define PWR_BASE_ADDRESS                 0x40007000U
#define DAC_BASE_ADDRESS                 0x40007400U

/***************************   APB2 Base Addresses    ******************************/

#define TIM1_BASE_ADDRESS             0x40010000U
#define TIM8_BASE_ADDRESS             0x40010400U
#define USART1_BASE_ADDRESS           0x40011000U
#define USART6_BASE_ADDRESS           0x40011400U
#define ADC_BASE_ADDRESS              0x40012000U
#define SDMMC_BASE_ADDRESS            0x40012C00U
#define SPI1_BASE_ADDRESS             0x40013000U
#define SPI4_BASE_ADDRESS             0x40013400U
#define SYSCFG_BASE_ADDRESS           0x40013800U
#define EXTI_BASE_ADDRESS             0x40013C00U
#define TIM9_BASE_ADDRESS             0x40014000U
#define TIM10_BASE_ADDRESS            0x40014400U
#define TIM11_BASE_ADDRESS            0x40014800U

/***************************   AHB1 Base Addresses    *****************************/

#define GPIOA_BASE_ADDRESS       0x40020000U
#define GPIOB_BASE_ADDRESS       0x40020400U
#define GPIOC_BASE_ADDRESS       0x40020800U
#define GPIOD_BASE_ADDRESS       0x40020C00U
#define GPIOE_BASE_ADDRESS       0x40021000U
#define GPIOF_BASE_ADDRESS       0x40021400U
#define GPIOG_BASE_ADDRESS       0x40021800U
#define GPIOH_BASE_ADDRESS       0x40021C00U

#define RCC_BASE_ADDRESS         0x40023800U

#define DMA1_BASE_ADDRESS        0x40026000U
#define DMA2_BASE_ADDRESS        0x40026400U

/*****************************   AHB2 Base Addresses    ******************************/

#define USB_OTG_FS_BASE          0x50000000U
#define DCMI_BASE_ADDRESS        0x50050000U

/****************************   AHB3 Base Addresses    ********************************/


/**********************   GPIO Register Definition Structure   ***************************/

typedef struct {
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];
}GPIO_Reg_t;

#define GPIOA          (GPIO_Reg_t*)GPIOA_BASE_ADDRESS
#define GPIOB          (GPIO_Reg_t*)GPIOB_BASE_ADDRESS
#define GPIOC          (GPIO_Reg_t*)GPIOC_BASE_ADDRESS
#define GPIOD          (GPIO_Reg_t*)GPIOD_BASE_ADDRESS
#define GPIOE          (GPIO_Reg_t*)GPIOE_BASE_ADDRESS
#define GPIOF          (GPIO_Reg_t*)GPIOF_BASE_ADDRESS
#define GPIOG          (GPIO_Reg_t*)GPIOG_BASE_ADDRESS
#define GPIOH          (GPIO_Reg_t*)GPIOH_BASE_ADDRESS

/***************************   NVIC Register Definition Structure   *****************************/

typedef struct {
	volatile uint32_t ISER[8];                     /*Interrupt Enable registers*/
	volatile uint32_t Reserved1[24];
	volatile uint32_t ICER[8];                     /*Interrupt Disable registers*/
	volatile uint32_t Reserved2[24];
	volatile uint32_t ISPR[8];                     /*Interrupt set pending registers*/
	volatile uint32_t Reserved3[24];
    volatile uint32_t ICPR[8];                     /*Interrupt clear pending registers*/
    volatile uint32_t Reserved4[24];
	volatile uint32_t IABR[8];                     /*Interrupt active bit registers*/
	volatile uint32_t Reserved5[56];
	volatile uint8_t IPR[240];                     /*Interrupt Priority registers*/
	volatile uint32_t Reserved6[643];
	volatile uint32_t STIR;
}NVIC_Reg_t;

/***************************   SCB Register Definition Structure   *****************************/

typedef struct {
	volatile uint32_t ACTLR[830];          /*Auxiliary Control Register*/
	volatile uint32_t CPUID;
	volatile uint32_t ICSR;                /*Interrupt Control and State Register*/
	volatile uint32_t VTOR;
	volatile uint32_t AIRCR;               /*Application Interrupt and Reset Control Register*/
	volatile uint32_t SCR;                 /* System Control Register  */
	volatile uint32_t CCR;
	volatile uint32_t SHPR1;               /* System Handler Priority Register 1 */
	volatile uint32_t SHPR2;               /* System Handler Priority Register 2 */
	volatile uint32_t SHPR3;               /* System Handler Priority Register 3 */
	volatile uint32_t SHCRS;               /* System Handler Control and State Register*/
	volatile uint32_t CFSR;                /* Configurable Fault Status Register(Mem-manage, Bus-fault, Usage)*/
	volatile uint32_t HFSR[2];             /* HardFault Status Register */
	volatile uint32_t MMAR;
	volatile uint32_t BFAR;
	volatile uint32_t AFSR;
}SCB_Reg_t;

/***************************   EXTI Register Definition Structure   *****************************/

typedef struct {
	volatile uint32_t IMR;               /* Interrupt mask Register */
	volatile uint32_t EMR;               /* Event mask Register */
	volatile uint32_t RTSR;              /* Rising trigger selection Register */
	volatile uint32_t FTSR;              /* Falling trigger selection Register */
	volatile uint32_t SWIER;             /* Software interrupt event Register */
	volatile uint32_t PR;                /* Pending Register */
}EXTI_Reg_t;

/**************************   SYSCFG Register Definition Structure   *****************************/

typedef struct {
	volatile uint32_t MEMRMP;                     /* Memory re-map Register */
	volatile uint32_t PMC;                        /* Peripheral mode configuration Register */
	volatile uint32_t EXTICR[4];                  /* External interrupt configuration Registers */
	volatile uint32_t Reserved1[2] ;
	volatile uint32_t CMPCR;                      /* Compensation cell control Register */
	volatile uint32_t Reserved2[2];
	volatile uint32_t CFGR;                       /* SYSCFG configuration Register */
}SYSCFG_Reg_t;


/****************************   DMA Register Definition Structure   *****************************/


typedef struct {
	volatile uint32_t SCR;                /* Stream configuration register */
	volatile uint32_t SNDTR;              /* Stream number of data register */
	volatile uint32_t SPAR;               /* Stream peripheral address register */
	volatile uint32_t SM0AR;              /* Stream memory 0 address register */
	volatile uint32_t SM1AR;              /* Stream memory 1 address register */
	volatile uint32_t SFCR;               /* Stream FIFO control register */
}DMA_Stream_Reg;

typedef struct {
	volatile uint32_t ISR[2];                /* Interrupt status register */
	volatile uint32_t IFCR[2];               /* Interrupt flag clear register */
	volatile DMA_Stream_Reg Stream[8];
}DMA_Reg_t;

#define DMA1        (DMA_Reg_t*)(DMA1_BASE_ADDRESS)
#define DMA2        (DMA_Reg_t*)(DMA2_BASE_ADDRESS)

/* stream configuration register SCR pins */
enum {
	Stream_EN, DMEIE, DMA_TEIE, HTIE, DMA_TCIE, PFCTRL, DIR, CIRC = 8, PINC, MINC, PSIZE, MSIZE = 13,
	PINCOS = 15, PL, DBM = 18, CT, PBURST = 21, MBURST = 23, CHSEL = 25
};

/* Stream FIFO control register SFCR pins */
enum {
	FTH, DMDIS = 2, FS, FEIE = 7
};


/**************************   RCC Register Definition Structure   ********************************/

typedef struct {
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	volatile uint32_t ReservedReg ;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t ReservedReg2;
	volatile uint32_t ReservedReg3;
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	volatile uint32_t ReservedReg4;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t ReservedReg5;
	volatile uint32_t ReservedReg6;
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t AHB3LPENR;
	volatile uint32_t ReservedReg7;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	volatile uint32_t ReservedReg8;
	volatile uint32_t ReservedReg9;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t ReservedReg10;
	volatile uint32_t ReservedRe11;
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t PLLSAICFGR;
	volatile uint32_t DCKCFGR;
	volatile uint32_t CKGATENR;
	volatile uint32_t DCKCFGR2;
}RCC_Reg_t;

/*Clock Control Register CR pins */
enum {
	HSION, HSIRDY, HSITRIM0 = 3, HSITRIM1, HSITRIM2, HSITRIM3, HSITRIM4, HSICAL0, HSICAL1,
	HSICAL2, HSICAL3, HSICAL4, HSICAL5, HSICAL6, HSICAL7, HSEON, HSERDY, HSEBYP, CSSON,
	PLLON = 24, PLLRDY, PLLI2SON, PLLI2SRDY, PLLSAION, PLLSAIRDY
};

/* Clock configuration register CFGR pins */
enum {
	SW0, SW1, SWS0, SWS1, HPRE0, HPRE1, HPRE2, HPRE3, PPRE10 = 10, PPRE11, PPRE12, PPRE20, PPRE21,
	PPRE22, RTCPRE0, RTCPRE1, RTCPRE2, RTCPRE3, RTCPRE4, MCO10, MCO11, MCO1PRE0 = 24, MCO1PRE1,
	MCO1PRE2, MCO2PRE0, MCO2PRE1, MCO2PRE2, MCO20, MCO21
};

/* PLL configuration register PLLCFGR pins */
enum {
	PLLM0, PLLM1, PLLM2, PLLM3, PLLM4, PLLM5, PLLN0, PLLN1, PLLN2, PLLN3, PLLN4, PLLN5, PLLN6,
	PLLN7, PLLN8, PLLP0 = 16, PLLP1, PLLSRC = 22, PLLQ0 = 24, PLLQ1, PLLQ2, PLLQ3, PLLR0, PLLR1, PLLR2
};

/**********************   SysTick Register Definition Structure   ******************************/

typedef struct {
	volatile uint32_t CSR;         /* Control and Status Register*/
	volatile uint32_t RVR;         /*Reload Value Register*/
	volatile uint32_t CVR;         /*Current value register*/
	volatile uint32_t CALIB;       /*Calibration Value Register*/
}SysTick_Reg_t;

/* SysTick control and status register CSR Pins  */
enum {
	ENABLE, TICKINT, CLKSOURCE, COUNTFLAG = 16
};

/**********************   USART Register Definition Structure   ***************************/

typedef struct {
	volatile uint32_t SR;              /*Status Register */
	volatile uint32_t DR;              /*Data Register */
	volatile uint32_t BRR;            /*BaudRate Register */
	volatile uint32_t CR1;            /*Control Register 1 */
	volatile uint32_t CR2;            /*Control Register 2 */
	volatile uint32_t CR3;            /*Control Register 3 */
	volatile uint32_t GTPR;          /* Guard time and pre-scaler register */
}USART_Reg_t;

#define USART1           (USART_Reg_t*)(USART1_BASE_ADDRESS)
#define USART2           (USART_Reg_t*)(USART2_BASE_ADDRESS)
#define USART3           (USART_Reg_t*)(USART3_BASE_ADDRESS)
#define UART4            (USART_Reg_t*)(UART4_BASE_ADDRESS)
#define UART5            (USART_Reg_t*)(UART5_BASE_ADDRESS)
#define USART6           (USART_Reg_t*)(USART6_BASE_ADDRESS)

/* Status register SR Pins  */
enum {
	PE, FE, NF, ORE, IDLE, RXNE, TC, TXE, LBD, CTS
};

/* Control register CR1 Pins  */
enum {
	SBK, RWU, RE, TE, IDLEIE, RXNEIE, TCIE, TXEIE, PEIE, PS, PCE, WAKE, M, UE, OVER8 = 15
};

/* Control register CR2 Pins  */
enum {
	ADD0, ADD1, ADD2, ADD3, LBDL = 5, LBDIE, LBCL = 8, CPHA, CPOL, CLKEN, STOP0, STOP1, LINEN
};

/* Control register CR3 Pins  */
enum {
	EIE, IREN, IRLP, HDSEL, NACK, SCEN, DMAR, DMAT, RTSE, CTSE, CTSIE, ONEBIT
};

/**************************   SPI Register Definition Structure   ***************************/

typedef struct {
	volatile uint32_t CR1;                 /* Control Register 1 */
	volatile uint32_t CR2;                 /* Control Register 1 */
	volatile uint32_t SR;                  /* Status Register */
	volatile uint32_t DR;                  /* Data Register */
	volatile uint32_t CRCPR;               /* CRC polynomial Register */
	volatile uint32_t RXCRCR;              /*  RX CRC Register */
	volatile uint32_t TXCRCR;              /*  TX CRC Register */
	volatile uint32_t I2SCFGR;              /* I2S Configuration Register */
	volatile uint32_t I2SPR;                /* I2S Pre-scaler Register */
}SPI_Reg_t;

#define SPI1             (SPI_Reg_t*)(SPI1_BASE_ADDRESS)
#define SPI2             (SPI_Reg_t*)(SPI2_BASE_ADDRESS)
#define SPI3             (SPI_Reg_t*)(SPI3_BASE_ADDRESS)
#define SPI4             (SPI_Reg_t*)(SPI4_BASE_ADDRESS)

  /* Control register CR1 pins*/
enum {
	SPI_CPHA, SPI_CPOL, MSTR, BR0, BR1, BR2, SPE, LSBFIRST, SSI, SSM, RXONLY, DFF, CRCNEXT, CRCEN, BIDIOE, BIDIMODE
};

/* Control register CR2 pins*/
enum {
	RXDMAEN, TXDMAEN, SSOE, FRF = 4, ERRIE, SPI_RXNEIE, SPI_TXEIE
};

/**************************   I2C Register Definition Structure   ***************************/

typedef struct {
	volatile uint32_t CR1;                  /* Control Register 1 */
	volatile uint32_t CR2;                  /* Control Register 2 */
	volatile uint32_t OAR1;                 /* Own address Register 1 */
	volatile uint32_t OAR2;                 /* Own address Register 2 */
	volatile uint32_t DR;                   /* Data Register */
	volatile uint32_t SR1;                  /* Status Register 1 */
	volatile uint32_t SR2;                  /* Status Register 2 */
	volatile uint32_t CCR;                  /* Clock control Register */
	volatile uint32_t TRISE;
	volatile uint32_t FLTR;
}I2C_Reg_t;

#define I2C1             (I2C_Reg_t*)(I2C1_BASE_ADDRESS)
#define I2C2             (I2C_Reg_t*)(I2C2_BASE_ADDRESS)
#define I2C3             (I2C_Reg_t*)(I2C3_BASE_ADDRESS)

/* Control register CR1 pins*/
enum {
	I2C_PE, SMBUS, SMBTYPE = 3, ENARP, ENPEC, ENGC, NOSTRETCH, START, STOP, ACK, POS, PEC, ALERT, SWRST = 15

};

/* Control register CR2 pins*/
enum {
	FREQ0, FREQ1, FREQ2, FREQ3, FREQ4, FREQ5, ITERREN = 8, ITEVTEN, ITBUFEN, DMAEN, LAST
};

/* Own address Register 1 pins */
enum {
	I2C_ADD0, I2C_ADD1, I2C_ADD2, I2C_ADD3, I2C_ADD4, I2C_ADD5, I2C_ADD6, I2C_ADD7, I2C_ADD8, I2C_ADD9, ADDMODE = 15
};

/* Own address Register 2 pins */
enum {
	ENDUAL, ADD2_1, ADD2_2, ADD2_3, ADD2_4, ADD2_5, ADD2_6, ADD2_7
};

/* Status Register 1 pins */
enum {
	SB, ADDR, BTF, ADD10, STOPF, I2C_RxNE = 6, I2C_TxE, BERR, ARLO, AF, I2C_OVR, PECERR, TIMEOUT = 14, SMBALERT
};

/* Status Register 2 pins */
enum {
	MSL, BUSY, TRA, GENCALL = 4, SMBDEFAULT, SMBHOST, DUALF, PEC0, PEC1, PEC2, PEC3, PEC4, PEC5, PEC6, PEC7
};

/* Clock control Register pins */
enum {
	CCR0, CCR1, CCR2, CCR3, CCR4, CCR5, CCR6, CCR7, CCR8, CCR9, CCR10, CCR11, DUTY = 14 ,F_S
};


#endif /* F4_REGISTERS_H_ */
