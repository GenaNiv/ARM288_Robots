/*
 * servo.h
 *
 *  Created on: Apr 6, 2016
 *      Author: mpost7
 */
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "Timer.h"

#ifndef SERVO_H_
#define SERVO_H_

#define PERIOD 320000 //44000 = (16M*20ms)/8
#define ZERO_DEGREE 16000 //(16M*1ms)/8 = 2000


#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80

void Servo_Init(void);
void Servo_Scan(void);
void Servo_Move(uint8_t degrees);

#endif /* SERVO_H_ */
