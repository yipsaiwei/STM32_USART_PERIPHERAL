/*
 * Rcc.c
 *
 *  Created on: Jul 13, 2021
 *      Author: Yip Sai Wei
 */
#include	"Rcc.h"


void	rccResetUnresetDevice(_IO_ uint32_t *io, int bitNum){
  *io |= RCC_RESET_DEVICE(bitNum);			//Reset
  *io &= RCC_SET_DEVICE(bitNum);			//Unreset
}


void	rccUnresetAndEnableGpio(RccGpio	rccGpio){
  rccResetUnresetDevice(ahb1ResetReg, rccGpio);
  rcc->AHB1ENR |= (1 << rccGpio);
  /*
	rcc->AHB1RSTR = RCC_RESET_DEVICE(rccGpio);		//Reset
	rcc->AHB1RSTR = RCC_SET_DEVICE(rcc);		//Unreset
	rcc->AHB1ENR = ;		//Enable clock gating
   */
}

void	rccUnresetAndEnableUsart(RccUsart	rccUsart){
  if(rccUsart == 17){
    rccResetUnresetDevice(apb1ResetReg, rccUsart);
    rcc->APB1ENR |= (1 << rccUsart);
  }
  else{
    rccResetUnresetDevice(apb2ResetReg, rccUsart);
    rcc->APB2ENR |= (1 << rccUsart);
  }
}

void  rccUnresetAndEnableTimer(RccTimer rccTimer){

  rccResetUnresetDevice((uint32_t *)(RccBaseAddress + (rccTimer >> 16)), (rccTimer & 0xF));
  if((rccTimer >> 16) == RCC_APB1){
    rcc->APB1ENR |= (1 << (rccTimer & 0xF));
  }else{
    rcc->APB2ENR |= (1 << (rccTimer & 0xF));
  }
}

void  rccUnresetAndEnableI2c(RccI2c rccI2c){
  rccResetUnresetDevice((uint32_t *)(RccBaseAddress + (rccI2c >> 16)), (rccI2c & 0xFF));
  rcc->APB1ENR |= (1 << (rccI2c & 0xFF));
}

void  rccUnresetAndEnableAdc(RccAdc rccAdc){
  rccResetUnresetDevice((uint32_t *)(RccBaseAddress +(rccAdc >> 16)), (rccAdc & 0xFF));
  rcc->APB2ENR |= (1 << (rccAdc & 0xFF));
}
