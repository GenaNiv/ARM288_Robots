/*
 * uartConfig.h
 *
 *  Created on: Mar 3, 2016
 *      Author: mpost7
 */

#ifndef UARTCONFIG_H_
#define UARTCONFIG_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>

void uartInt(void);
void uartTransmit(char data);
#endif /* UARTCONFIG_H_ */
