/*
 * ping.c
 *
 *  Created on: Mar 23, 2016
 *      Author: mpost7
 */
#include "ping.h"


volatile uint16_t risingEdge;
volatile uint16_t fallingEdge;
volatile uint32_t pulseLength;
volatile uint8_t pingStartFlag;
volatile uint8_t pingTriggerFlag;
volatile uint8_t overflows;

void Ping_Init()
{



	SYSCTL_RCGCGPIO_R = BIT5; //Turn on PORTF Sys Clk

	GPIO_PORTF_DEN_R &= ~BIT3;
	//Set port to output
	GPIO_PORTF_DIR_R |= (BIT3 | BIT2); //Pin 3 is T1CCP1 Pin 2 is blue LED
	GPIO_PORTF_DEN_R |= (BIT3 | BIT2); //Pin 3 Enabled

	//Pulse Ping Sensor High to Low with ~ 5us delay
	GPIO_PORTF_DATA_R |= BIT3; //set port F data high
	timer_waitMicros(15);
	GPIO_PORTF_DATA_R &= ~BIT3; //set port F data low
	overflows = 0; //reset any timeout events
	pingTriggerFlag = 1; //flag to capture rising edge

}

void Ping_TimCapInit()
{
	//uint32_t timerCLK; //Set to 250KHZ
	//timerCLK = ( SYSCTL_RCC_PWRDN | SYSCTL_RCC_BYPASS | SYSCTL_RCC_USESYSDIV);
	//SYSCTL_RCC_R |= timerCLK;
	//SYSCTL_RCC2_R |= (SYS_DIV2_64 | SYSCTL_RCC2_USERCC2); //set divisor to 64 and enable reg

	SYSCTL_RCGCTIMER_R = BIT1; // Turn on clock to Timer1
	TIMER1_CTL_R &= ~TIMER_CTL_TBEN; //disable timerB
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
	TIMER1_TBMR_R |= (TIMER_TBMR_TBMR_CAP | TIMER_TBMR_TBCDIR | TIMER_TBMR_TBCMR); //set for capture mode, up count
	TIMER1_CTL_R |= TIMER_CTL_TBEVENT_BOTH; //set bits 11:10 for both edge triggering
	TIMER1_TBILR_R = 0xFFFF; // set top counter to maximum 16 bit value ~4.1ms @16MHz
	TIMER1_TBPR_R = 64; //16M/16 = 1us count max = 65.5 ms
	//clear Timer1B interrupt flags
	TIMER1_ICR_R = (TIMER_ICR_TBMCINT | TIMER_ICR_CBECINT | TIMER_ICR_CBMCINT | TIMER_ICR_TBTOCINT);
	TIMER1_IMR_R |= (TIMER_IMR_CBEIM | TIMER_IMR_TBTOIM); //enable Timer1B capture and time-out interrupts

	//intialize local interrupts
	NVIC_PRI5_R |= 0x00200000; //give priority 1 to interrupt 22(T1CCP1) set bits 23:21 to 1
	NVIC_EN0_R = 0x00400000; //enable interrupt for IRQ22(T1CCP1) set bit 22 a write of 0 has no effect

	//register ISR
	IntRegister(INT_TIMER1B, TIMER1B_Handler);

	//intialize global interrupts
	Enable_IRQ();
	IntMasterEnable();
	//enable timer1B and start counting
	TIMER1_CTL_R |= TIMER_CTL_TBEN;


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
	//check to see if capture event triggered interrupt
	GPIO_PORTF_DATA_R |= BIT2; //set PIN2 high and light blue led

	if(TIMER_MIS_CBMMIS)
		if(pingTriggerFlag)
		{
			TIMER1_ICR_R = TIMER_ICR_CBECINT; //clear capture interrupt flag
			risingEdge = TIMER1_TBR_R; //captures time of rising edge event
			pingStartFlag = 1; //flag to indicate pulse has started
			pingTriggerFlag = 0; //clear flag to capture falling edge
			GPIO_PORTF_DATA_R &= ~BIT2; //set port F data low
		}
		else
		{
			TIMER1_ICR_R = TIMER_ICR_CBECINT; //clear capture interrupt flag
			fallingEdge = TIMER1_TBR_R; //capture time of falling edge
			pingStartFlag = 0; //ping has ended clear flag
			GPIO_PORTF_DATA_R &= ~BIT2;
		}
	//check to see if timer1B has timed out
	if(TIMER_MIS_TBTOMIS)
	{
		TIMER1_ICR_R = TIMER_ICR_TBTOCINT; //clear timeout flag
		//check if a ping has been sent
		if(pingStartFlag)
		{
			overflows++;
			GPIO_PORTF_DATA_R &= ~BIT2;
		}
	}
}
float Ping_getDistance()
{
	float distance = 0;

	//make sure overflow is true correct by subtracting 1
	overflows -= (fallingEdge < risingEdge);

	//get pulse length
	pulseLength = (fallingEdge - risingEdge) + (overflows << 16);

	//calculate distance in cm
	distance = pulseLength * PING_CONVERSION;

 return distance;
}
