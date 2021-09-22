/*
 * Gpio.c
 *
 *  Created on: Jun 29, 2021
 *      Author: Yip Sai Wei
 */

#include	"Gpio.h"

void	gpioWritePin(Gpio  *gpio, int  pin, int  state){
	if(state)
		gpio->BSRR = state << pin;
	else
		gpio->BSRR = 1 << (16 + pin);
}

int	gpioReadPin(Gpio  *gpio, int  pin){
	return	(gpio->IDR >> pin) & 1;
}

void  gpioConfigurePin(Gpio *gpio, int  pin, GpioConfig config){
  gpio->MODER &= GPIO_MODE_MASK(pin);
  gpio->MODER |= (config & 0xF)  << (2 * pin);

  gpio->OTYPER &= GPIO_TYPE_MASK(pin);
  gpio->OTYPER |= ((config & 0xFF) >> 0x4) << pin;

  gpio->OSPEEDR &= GPIO_SPEED_MASK(pin);
  gpio->OSPEEDR |= ((config & 0xFFF) >> 0x8) << (2 * pin);

  gpio->PUPDR &= GPIO_PULL_MASK(pin);
  gpio->PUPDR |= ((config & 0xFFFF) >> 0xC) << (2 * pin);

  if(pin > 7){
    gpio->AFR[1] &= GPIO_AFR_MASK(pin - 8);
    gpio->AFR[1] |= ((config & 0xFFFFF) >> 0x10) << (4 * (pin - 8));
  }else{
    gpio->AFR[0] &= GPIO_AFR_MASK(pin);
    gpio->AFR[0] |= ((config & 0xFFFFF) >> 0x10) << (4 * pin);
  }
}
