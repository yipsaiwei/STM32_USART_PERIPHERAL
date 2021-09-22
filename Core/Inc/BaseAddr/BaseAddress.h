/*
 * BaseAddress.h
 *
 *  Created on: Aug 17, 2021
 *      Author: Yip Sai Wei
 */

#ifndef INC_BASEADDRESS_BASEADDRESS_H_
#define INC_BASEADDRESS_BASEADDRESS_H_

#define   PORT_A  0x40020000
#define   PORT_B  0x40020400
#define   PORT_C  0x40020800
#define   PORT_D  0x40020C00
#define   PORT_E  0x40021000
#define   PORT_H  0x40021C00

#define   NVIC_BASE_ADDRESS   0xE000E100

#define RccBaseAddress  0x40023800

#define TIM1BaseAddress     0x40010000
#define TIM2BaseAddress     0x40000000
#define TIM3BaseAddress     0x40000400
#define TIM4BaseAddress     0x40000800
#define TIM5BaseAddress     0x40000C00

#define Usart1BaseAddress 0x40011000
#define Usart2BaseAddress 0x40004400
#define Usart6BaseAddress 0x40011400

#define I2c1BaseAddress   0x40005400
#define I2c2BaseAddress   0x40005800
#define I2c3BaseAddress   0x40005C00

#define Adc1BaseAddress   0x40012000

#endif /* INC_BASEADDRESS_BASEADDRESS_H_ */
