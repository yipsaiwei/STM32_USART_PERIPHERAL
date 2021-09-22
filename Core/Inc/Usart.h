/*
 * Usart.h
 *
 *  Created on: Jul 14, 2021
 *      Author: Yip Sai Wei
 */

#ifndef INC_USART_H_
#define INC_USART_H_

#include  <stdint.h>
#include  "IO.h"
#include "stm32f4xx_hal.h"
#include <BaseAddr/BaseAddress.h>

#define	usart1				((UsartReg	*)Usart1BaseAddress)
#define	usart2				((UsartReg	*)Usart2BaseAddress)
#define	usart6				((UsartReg	*)Usart6BaseAddress)

#define USART_CR_MASK  ~(0xFFFF)
#define USART_DR_MASK  ~(0xFF)
//Control 1 CONFIGS
/*
#define USART_OVERSAMPLING_8  (1 << 15)
#define USART_USART_EN        (1 << 13)
#define USART_ENABLE_9_BIT    (1 << 12)
#define USART_ADDRESS_MARK    (1 << 11)
#define USART_ENABLE_PARITY   (1 << 10)
#define USART_ODD_PARITY      (1 << 9)
#define USART_PEIE            (1 << 8)
#define USART_TXEIE           (1 << 7)
#define USART_TCIE            (1 << 6)
#define USART_RXNEIE          (1 << 5)
#define USART_IDLEIE          (1 << 4)
#define USART_TE              (1 << 3)
#define USART_RE              (1 << 2)
#define USART_RWU             (1 << 1)
#define USART_SBK             (1 << 0)
*/

//Control 2 CONFIGS
/*
#define USART_LINEN           (1 << 14)
#define USART_STOP_BIT_1      (0 << 12)
#define USART_STOP_BIT_2      (2 << 12)
#define USART_CLKEN           (1 << 11)
#define USART_CPOL            (1 << 10)
#define USART_CPHA            (1 << 9)
#define USART_LBCL            (1 << 8)
#define USART_LBDIE           (1 << 6)
#define USART_LBDL            (1 << 5)
*/

//Control 3 CONFIGS
/*
#define USART_ONEBIT          (1 << 11)
#define USART_CTSIE           (1 << 10)
#define USART_CTSE            (1 << 9)
#define USART_RTSE            (1 << 8)
#define USART_DMAT            (1 << 7)
#define USART_DMAR            (1 << 6)
#define USART_SCEN            (1 << 5)
#define USART_NACK            (1 << 4)
#define USART_HDSEL           (1 << 3)
#define USART_IRLP            (1 << 2)
#define USART_IREN            (1 << 1)
#define USART_EIE             (1 << 0)
*/

typedef struct	UsartReg_t	UsartReg;
struct	UsartReg_t {
	_IO_ uint32_t	SR;
	_IO_ uint32_t	DR;
	_IO_ uint32_t	BRR;
	_IO_ uint32_t	CR1;
	_IO_ uint32_t	CR2;
	_IO_ uint32_t	CR3;
	_IO_ uint32_t	GTPR;
};

typedef enum{
  NOT_OK,
  OK,
}Status;

typedef enum{
  //Control Register 1
  USART_SEND_BREAK = 1,
  USART_RWU_MUTE        = 1 << 1,
  USART_RX_EN           = 1 << 2,
  USART_TX_EN           = 1 << 3,
  USART_IDLEIE_EN       = 1 << 4,
  USART_RXNEIE_EN       = 1 << 5,
  USART_TCIE_EN         = 1 << 6,
  USART_TXEIE_EN        = 1 << 7,
  USART_PEIE_EN         = 1 << 8,
  USART_ODD_PARITY      = 3 << 9,    USART_EVEN_PARITY = 2 << 9,
  USART_DISABLE_PARITY  = 0 << 10,   USART_ENABLE_PARITY =  1 << 10,
  USART_IDLE_LINE       = 0 << 11,   USART_ADDRESS_MARK = 1 << 11,
  USART_8_BIT           = 0 << 12,   USART_9_BIT = 1 << 12,
  USART_EN = 1 << 13,
  USART_OVERSAMPLING_8 = 0 << 15,    USART_OVERSAMPLING_16 = 1 << 15,

  //Control Register 2
  USART_LBDL_11_BIT = 1 << (5 + 16),
  USART_LBDIE_EN    = 1 << (6 + 16),
  USART_LBCL        = 1 << (8 + 16),
  USART_CPHA        = 1 << (9 + 16),
  USART_CPOL_HI     = 1 << (10 + 16),
  USART_CLKEN_EN    = 1 << (11 + 16),
  USART_1_STOPBIT   = 0 << (12 + 16), USART_0_5_STOPBIT = 1 << (12 + 16),
  USART_2_STOPBIT   = 2 << (12 + 16), USART_2_5_STOPBIT = 3 << (12 + 16),
  USART_LINEN_EN    = 0 << (14 + 16) ,

  //Control register 3
  USART_EIE_EN        = (uint64_t) 1 << (0 + 32),
  USART_IREN_EN       = (uint64_t) 1 << (1 + 32),
  USART_IRLP_LO       = (uint64_t) 1 << (2 + 32),
  USART_HDSEL         = (uint64_t) 1 << (3 + 32),
  USART_NACK_EN       = (uint64_t) 1 << (4 + 32),
  USART_SCEN_EN       = (uint64_t) 1 << (5 + 32),
  USART_DMAR_EN       = (uint64_t) 1 << (6 + 32),
  USART_DMAT_EN       = (uint64_t) 1 << (7 + 32),
  USART_RTSE_EN       = (uint64_t) 1 << (8 + 32),
  USART_CTSE_EN       = (uint64_t) 1 << (9 + 32),
  USART_CTSIE_EN      = (uint64_t) 1 << (10 + 32),
  USART_ONEBIT_SAMPLE = (uint64_t) 1 << (11 + 32),
}UsartConfig;
void  usartSetBaudRate(UsartReg *usart, int baudrate);
//void  usartConfigureCR1(UsartReg *usart, int config);
//void  usartConfigureCR2(UsartReg *usart, int config);
//void  usartConfigureCR3(UsartReg *usart, int config);
void  usartConfigure(UsartReg *usart, UsartConfig config);
void  writeToDataRegister(UsartReg *usart, uint8_t  data);
int isTDREmpty(UsartReg *usart);
int isTransmissionComplete(UsartReg *usart);
int isRDRAvailableForRead(UsartReg *usart);
int returnRDRValue(UsartReg *usart);
uint32_t returnUsartFrequency(UsartReg *usart);
void  usartConfiguration(UsartReg *usart, UsartConfig config, int baudrate);
Status  usartTransmit(UsartReg  *usart, char  *msg);
Status  usartTransmitWithLength(UsartReg  *usart, char  *msg, int len);
Status readFromUsart(UsartReg  *usart, char  *msg);

#endif /* INC_USART_H_ */
