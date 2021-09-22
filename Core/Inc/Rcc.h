/*
 * Rcc.h
 *
 *  Created on: Jul 6, 2021
 *      Author: Yip Sai Wei
 */

#ifndef INC_RCC_H_
#define INC_RCC_H_

#include <BaseAddr/BaseAddress.h>
#include	<stdint.h>
#include	"IO.h"

#define	ahb1ResetReg			      (&rcc->AHB1RSTR)
#define	ahb2ResetReg			      (&rcc->AHB2RSTR)
#define	apb1ResetReg			      (&rcc->APB1RSTR)
#define	apb2ResetReg			      (&rcc->APB2RSTR)

#define	RCC_SET_DEVICE(PIN)		  (~(1 << PIN))
#define	RCC_RESET_DEVICE(PIN)	  (1 << PIN)

typedef enum{
  RCC_AHB  = 0x10,
  RCC_AHB1 = 0x10,
  RCC_AHB2 = 0x14,
  RCC_APB  = 0x20,
  RCC_APB1 = 0x20,
  RCC_APB2 = 0x24,

}RccAdvancedBusOffset;
typedef	enum{
	RCC_GPIOA,
	RCC_GPIOB,
	RCC_GPIOC,
	RCC_GPIOD,
	RCC_GPIOE,
	RCC_GPIOF = 7,
}RccGpio;

typedef	enum{
	RCC_USART1 = 4,
	RCC_USART2 = 17,
	RCC_USART6 = 5,
}RccUsart;

typedef enum{
  RCC_TIM1 = 0       | (RCC_APB2 << 16),
  RCC_TIM2 = 0       | (RCC_APB1 << 16),
  RCC_TIM3 = 1       | (RCC_APB1 << 16),
  RCC_TIM4 = 2       | (RCC_APB1 << 16),
  RCC_TIM5 = 3       | (RCC_APB1 << 16),
  RCC_TIM9 = 16      | (RCC_APB2 << 16),
  RCC_TIM10 = 17     | (RCC_APB2 << 16),
  RCC_TIM11 = 18     | (RCC_APB2 << 16),
}RccTimer;

typedef enum{
  RCC_I2C1 = 21     | (RCC_APB1 << 16),
  RCC_I2C2 = 22     | (RCC_APB1 << 16),
  RCC_I2C3 = 23     | (RCC_APB1 << 16),
}RccI2c;

typedef enum{
  RCC_ADC1 = 8     | (RCC_APB2 << 16),
}RccAdc;

#define	rcc				      ((RccReg	*)RccBaseAddress)

typedef struct	RccReg_t	RccReg;
struct	RccReg_t {
	_IO_ uint32_t	CR;
	_IO_ uint32_t	PLLCFGR;
	_IO_ uint32_t	CFGR;
	_IO_ uint32_t	CIR;
	_IO_ uint32_t	AHB1RSTR;
	_IO_ uint32_t	AHB2RSTR;
	_IO_ uint32_t	reserved0[2];
	_IO_ uint32_t	APB1RSTR;
	_IO_ uint32_t	APB2RSTR;
	_IO_ uint32_t	reserved1[2];
	_IO_ uint32_t	AHB1ENR;
	_IO_ uint32_t	AHB2ENR;
	_IO_ uint32_t	reserved2[2];
	_IO_ uint32_t	APB1ENR;
	_IO_ uint32_t	APB2ENR;
	_IO_ uint32_t	reserved3[2];
	_IO_ uint32_t	AHB1LPENR;
	_IO_ uint32_t	AHB2LPENR;
	_IO_ uint32_t	reserved4[2];
	_IO_ uint32_t	APB1LPENR;
	_IO_ uint32_t	APB2LPENR;
	_IO_ uint32_t	reserved5[2];
	_IO_ uint32_t	BDCR;
	_IO_ uint32_t	CSR;
	_IO_ uint32_t	reserved6[2];
	_IO_ uint32_t	SSCGR;
	_IO_ uint32_t	PLLI2SCFGR;
	_IO_ uint32_t	reserved7;
	_IO_ uint32_t	DCKCFGR;
};

void	rccResetUnresetDevice(_IO_ uint32_t *io, int bitNum);
void	rccUnresetAndEnableGpio(RccGpio	rccGpio);
void	rccUnresetAndEnableUsart(RccUsart	rccUsart);
void  rccUnresetAndEnableTimer(RccTimer rccTimer);
void  rccUnresetAndEnableI2c(RccI2c rccI2c);
void  rccUnresetAndEnableAdc(RccAdc rccAdc);
/*
//0x10	RCC_AHB1RSTR
//0x30	RCC_AHB1ENR
#define	RccBaseAddress	0x40023800

#define	rcc_ahb1rstr	(*(_IO_ int32_t	*)(RccBaseAddress + 0x10))
#define	rcc_ahb1enr		(*(_IO_ int32_t	*)(RccBaseAddress + 0x30))

*/
#endif /* INC_RCC_H_ */
