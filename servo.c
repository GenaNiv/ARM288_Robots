/*
 * servo.c
 *
 *  Created on: Apr 6, 2016
 *      Author: mpost7
 */

#include "servo.h"

void Servo_Init()
{
	SYSCTL_RCGCGPIO_R |= BIT5; //turn on clk to PORTF
	GPIO_PORTF_AFSEL_R |= BIT1;
	GPIO_PORTF_PCTL_R |= 0x00000070; //enable pin1 alternate function 7 T0CCP1
	GPIO_PORTF_DIR_R |= BIT1; //set for output
	GPIO_PORTF_DEN_R |= BIT1;
	SYSCTL_RCGCTIMER_R |= BIT0; //turn on clk for T0CCP1/PWM1: PF1

	//Configure T0CCP1 as PWM1
	TIMER0_CTL_R &= ~TIMER_CTL_TBEN; //disable timerB
	//TIMER0_CFG_R |= TIMER_CFG_32_BIT_TIMER;
	TIMER0_CFG_R |= TIMER_CFG_16_BIT;

	//set alt sel to PWM and Periodic timer mode
	//TIMER0_TBMR_R |= (TIMER_TBMR_TBAMS | TIMER_TBMR_TBMR_PERIOD);
	TIMER0_TBMR_R = TIMER_TBMR_TBMR_PERIOD | TIMER_TBMR_TBAMS;
	TIMER0_CTL_R &= TIMER_CTL_TBPWML; //Invert the PWM logic
	//TIMER0_TBMR_R &= ~TIMER_TBMR_TBCMR; //clear cap mode to edge count
	//TIMER0_TBMR_R |= TIMER_TBMR_TBMR_PERIOD; //Set periodic mode
	TIMER0_TBPR_R = (PERIOD >> 16) & 0xFF; //  Holds top 8 bits of prescalar
	TIMER0_TBILR_R = (PERIOD & 0xFFFF); //set period of PWM

	TIMER0_TBPMR_R = (ZERO_DEGREE >> 16) & 0xFF;
	TIMER0_TBMATCHR_R = ZERO_DEGREE & 0xFFFF;

	TIMER0_CTL_R |= TIMER_CTL_TBEN; //enable PWM ---- remove for later
}

void Servo_Move(uint8_t degrees)
{
	if(degrees >= 180) degrees = 180;
	else if(degrees <= 0) degrees = 0;

	uint32_t match = PERIOD - (ZERO_DEGREE + (MAX_DEGREE - ZERO_DEGREE) * (uint32_t)degrees / 180);

	TIMER0_TBPMR_R = (match >> 16) & 0xFF;	//Bits 23:16 of match value
	TIMER0_TBMATCHR_R = match & 0xFFFF;		//Bits 15:0 of match value
}
void Servo_Scan()
{
	uint32_t i, count;
	count = 39000;
	for(i = 0; i < 39000; i++)
	{
		TIMER0_TBMATCHR_R = count;
		count = count - 1000;
	}
}

void Servo_Test() {
	Servo_Init();

	while(1)
	{
		static int degrees = 0;
		static int dir = 1;

		Servo_Move(degrees += dir);

		if(degrees > 180) {
			degrees = 180;
			dir = -1;
		}
		else if(degrees < 0) {
			degrees = 0;
			dir = 1;
		}

		volatile uint32_t i;
		for(i = 50000; i > 0; i--);
	}
}

