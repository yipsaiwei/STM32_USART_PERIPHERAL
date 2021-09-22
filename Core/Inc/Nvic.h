/*
 * Nvic.h
 *
 *  Created on: Aug 10, 2021
 *      Author: Yip Sai Wei
 */

#ifndef INC_NVIC_H_
#define INC_NVIC_H_

#include <BaseAddr/BaseAddress.h>
#include  <stdint.h>
#include  "IO.h"
#define   nvic                ((NvicReg *)NVIC_BASE_ADDRESS)

typedef struct  NvicReg_t  NvicReg;
struct  NvicReg_t {
  _IO_ uint32_t  ISER[8];
  _IO_ uint32_t  reserved0[24];
  _IO_ uint32_t  ICER[8];
  _IO_ uint32_t  reserved1[24];
  _IO_ uint32_t  ISPR[8];
  _IO_ uint32_t  reserved2[24];
  _IO_ uint32_t  ICPR[8];
  _IO_ uint32_t  reserved3[24];
  _IO_ uint32_t  IABRR[8];
  _IO_ uint32_t  reserved4[56];
  _IO_ uint32_t  IPR[60];
  _IO_ uint32_t  reserved5[644];
  _IO_ uint32_t  STIR;
};

typedef enum{
  ADC_IRQ    = 18,
  USART1_IRQ = 37,
  USART2_IRQ = 38,
  USART6_IRQ = 71,
}IrqNum;

#define nvicEnableIrq(n)                        \
    nvic->ISER[n >> 5] |= 1 << ((n) & 0x1f)

#define nvicDisableIrq(n)                       \
    nvic->ICER[n >> 5] |= 1 << ((n) & 0x1f)

#define nvicSetPendingIrq(n)                    \
    nvic->ISPR[n >> 5] |= 1 << ((n) & 0x1f)

#define nvicClearPendingIrq(n)                  \
    nvic->ICPR[n >> 5] |= 1 << ((n) & 0x1f)

#define nvicSetPriority(n, priority)            \
    nvic->IPR[n >> 2] |= (priority << ((n & 0x3) * 8)) << 4

#define nvicTriggerIrq(n)                       \
  nvic->STIR = n
#endif /* INC_NVIC_H_ */
