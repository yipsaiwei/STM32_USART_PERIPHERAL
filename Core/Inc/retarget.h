/*
 * retarget.h
 *
 *  Created on: Sep 14, 2021
 *      Author: Yip Sai Wei
 */

#ifndef INC_RETARGET_H_
#define INC_RETARGET_H_

#include "stm32f4xx_hal.h"
#include <sys/stat.h>
#include  "Usart.h"

void RetargetInit(UsartReg *usart);
int _isatty(int fd);
int _write(int fd, char* ptr, int len);
int _close(int fd);
int _lseek(int fd, int ptr, int dir);
int _read(int fd, char* ptr, int len);
int _fstat(int fd, struct stat* st);

#endif /* INC_RETARGET_H_ */
