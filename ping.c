/*
 * ping.c
 *
 *  Created on: Mar 23, 2016
 *      Author: mpost7
 */
#include "ping.h"

volatile uint16_t risingEdge = 0;
volatile uint16_t fallingEdge = 0;
volatile uint8_t pingStartFlag = 0;
volatile uint8_t pingTriggerFlag = 0;

void ping_Init()
{
	SYSCTL_RCGCGPIO_R = BIT5; //Turn on PORTF Sys Clk

	//Set port to output
	GPIO_PORTF_DIR_R |= BIT3; //Pin 3 to output
	GPIO_PORTF_DEN_R |= BIT3; //Pin 3 Enabled

	//Pulse Ping Sensor High to Low with ~ 5us delay
	GPIO_PORTF_DATA_R |= 0xFF; //set port F data high
	timer_waitMicros(5);
	GPIO_PORTF_DATA_R &= 0xFFFFFF00; //set port F data low
	pingTriggerFlag = 1; //flag to capture rising edge
}

void ping_TimCapInit()
{
	uint32_t timerCLK;
	timerCLK = (SYSCTL_RCC_XTAL_16MHZ | SYSCTL_RCC_PWRDN | SYSCTL_RCC_BYPASS | SYSCTL_RCC_SYSDIV_M);
	SYSCTL_RCC_R = timerCLK; //ensure that timers are running at exactly 16MHz
	SYSCTL_RCGCTIMER_R = BIT5; // Turn on clock to Timer1

	//set to pin 3 to timer capture and input
	//may not need to disable pin 3 before changing
	GPIO_PORTF_DEN_R &= ~BIT3;
	GPIO_PORTF_AFSEL_R |= BIT3;
	GPIO_PORTF_PCTL_R |= 0x00007000; //enable pin3 alternate function 7 T1CCP1
	GPIO_PORTF_DIR_R &= ~BIT3; //set 0 for input
	GPIO_PORTF_DEN_R |= BIT3; //re-enable pin 3

	//Configure TI1CCP1
	TIMER1_CTL_R &= ~TIMER_CTL_TBEN; //disable timerB
	TIMER1_CFG_R |= TIMER_CFG_16_BIT; //set to 16 bit timer
	TIMER1_TBMR_R |= (TIMER_TBMR_TBMR_CAP | TIMER_TBMR_TBCDIR); //set for capture mode, up count
	TIMER1_CTL_R |= TIMER_CTL_TBEVENT_BOTH; //set bits 11:10 for both edge triggering
	//TIMER1_TBPR_R |= TIMER_TBPR_TBPSR_M; //enable 8 bit prescaler to make a 24 bit counter
	TIMER1_TBILR_R = 0xFFFF; // set top counter to maximum 16 bit value ~4.1ms @16MHz

	//clear Timer1B interrupt flags
	TIMER1_ICR_R = (TIMER_ICR_TBMCINT | TIMER_ICR_CBECINT | TIMER_ICR_CBMCINT | TIMER_ICR_TBTOCINT);
	TIMER1_IMR_R |= (TIMER_IMR_CBEIM | TIMER_IMR_TBTOIM); //enable Timer1B capture and time-out interrupts

	//intialize local interrupts
	NVIC_PRI5_R |= 0x00200000; //give priority 1 to interrupt 22(T1CCP1) set bits 23:21 to 1
	NVIC_EN0_R = 0x00400000; //enable interrupt for IRQ22(T1CCP1) set bit 22 a write of 0 has no effect

	//intialize global interrupts
	Enable_IRQ();
}

void Enable_IRQ()
{
	int R2;
	__asm (" MOV R2, #0\n\t"
			" MSR BASEPRI, R2\n\t"
			" CPSIE I");
}

void TIMER1B_Handler(void)
{
	if(pingTriggerFlag)
	{
		TIMER1_ICR_R = TIMER_ICR_CBECINT; //clear capture interrupt flag
		risingEdge = TIMER1_TBR_R; //captures time of rising edge event
		pingStartFlag = 1; //flag to indicate pulse has started
		pingTriggerFlag = 0; //clear flag to capture falling edge

	}

}
