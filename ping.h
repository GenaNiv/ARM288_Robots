/*
 * ping.h
 *
 *  Created on: Mar 23, 2016
 *      Author: mpost7
 */

#ifndef PING_H_
#define PING_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "Timer.h"
#include "driverlib/interrupt.h"




#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80
#define PING_CONVERSION 0.17

void Ping_Init(void);
void Ping_TimCapInit(void);
void Enable_IRQ(void);
void TIMER1B_Handler(void);
float Ping_getDistance(void);

#endif /* PING_H_ */
