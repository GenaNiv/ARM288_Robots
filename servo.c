/*
 * servo.c
 *
 *  Created on: Apr 6, 2016
 *      Author: mpost7
 */

#include "servo.h"

void Servo_Init()
{
	SYSCTL_RCGCGPIO_R = BIT5; //turn on clk to PORTF
	GPIO_PORTF_AFSEL_R |= BIT1;
	GPIO_PORTF_PCTL_R |= 0x00000070; //enable pin1 alternate function 7 T0CCP1
	GPIO_PORTF_DIR_R |= BIT1; //set for output
	GPIO_PORTF_DEN_R |= BIT1;
	SYSCTL_RCGCTIMER_R = BIT0; //turn on clk for T0CCP1/PWM1: PF1

	//Configure T0CCP1 as PWM1
	TIMER0_CTL_R &= ~TIMER_CTL_TBEN; //disable timerB
	TIMER0_CFG_R |= TIMER_CFG_32_BIT_TIMER;
	//set alt sel to PWM and Periodic timer mode
	TIMER0_TBMR_R |= (TIMER_TBMR_TBAMS | TIMER_TBMR_TBMR_PERIOD);
	TIMER0_CTL_R &= ~TIMER_CTL_TBPWML;
	TIMER0_TBMR_R &= ~TIMER_TBMR_TBCMR; //clear cap mode to edge count
	//TIMER0_TBPR_R |= 0xFF; // 8 bit prescaler
	TIMER0_TBILR_R = PERIOD; //set period of PWM
	//TIMER0_TBMATCHR_R = 42000;
	TIMER0_CTL_R |= TIMER_CTL_TBEN; //enable PWM
}

void Servo_Move(uint8_t degrees)
{
	if(degrees >= 180) degrees = 180;
	else if(degrees <= 0) degrees = 0;

	uint32_t pulseWidth = (degrees * 177.777) + ZERO_DEGREE; //(16M clk * 2ms)/180 = 22.222
	TIMER0_TBMATCHR_R = PERIOD - pulseWidth;

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

