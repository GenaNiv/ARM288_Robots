/*
 * Timer.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: ericm
 */

#include "Timer.h"

volatile uint32_t _timer_ticks;

/*
 * Used for reference -- Delete eventually
#define TIMER0_CFG_R            (*((volatile uint32_t *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile uint32_t *)0x40030004))
#define TIMER0_TBMR_R           (*((volatile uint32_t *)0x40030008))
#define TIMER0_CTL_R            (*((volatile uint32_t *)0x4003000C))
#define TIMER0_SYNC_R           (*((volatile uint32_t *)0x40030010))
#define TIMER0_IMR_R            (*((volatile uint32_t *)0x40030018))
#define TIMER0_RIS_R            (*((volatile uint32_t *)0x4003001C))
#define TIMER0_MIS_R            (*((volatile uint32_t *)0x40030020))
#define TIMER0_ICR_R            (*((volatile uint32_t *)0x40030024))
#define TIMER0_TAILR_R          (*((volatile uint32_t *)0x40030028))
#define TIMER0_TBILR_R          (*((volatile uint32_t *)0x4003002C))
#define TIMER0_TAMATCHR_R       (*((volatile uint32_t *)0x40030030))
#define TIMER0_TBMATCHR_R       (*((volatile uint32_t *)0x40030034))
#define TIMER0_TAPR_R           (*((volatile uint32_t *)0x40030038))
#define TIMER0_TBPR_R           (*((volatile uint32_t *)0x4003003C))
#define TIMER0_TAPMR_R          (*((volatile uint32_t *)0x40030040))
#define TIMER0_TBPMR_R          (*((volatile uint32_t *)0x40030044))
#define TIMER0_TAR_R            (*((volatile uint32_t *)0x40030048))
#define TIMER0_TBR_R            (*((volatile uint32_t *)0x4003004C))
#define TIMER0_TAV_R            (*((volatile uint32_t *)0x40030050))
#define TIMER0_TBV_R            (*((volatile uint32_t *)0x40030054))
#define TIMER0_RTCPD_R          (*((volatile uint32_t *)0x40030058))
#define TIMER0_TAPS_R           (*((volatile uint32_t *)0x4003005C))
#define TIMER0_TBPS_R           (*((volatile uint32_t *)0x40030060))
#define TIMER0_TAPV_R           (*((volatile uint32_t *)0x40030064))
#define TIMER0_TBPV_R           (*((volatile uint32_t *)0x40030068))
#define TIMER0_PP_R             (*((volatile uint32_t *)0x40030FC0))
 */

void timer_waitMillis(uint32_t millis) {
	//Start timer with period of 1ms
	timer_startTimer(999);

	while(millis > 0) {
		while(!(TIMER0_RIS_R & TIMER_RIS_TATORIS));

		TIMER0_ICR_R |= TIMER_RIS_TATORIS;
		millis--;
	}

	timer_stopTimer();
}

void timer_waitMicros(uint16_t micros) {
	//Start timer with period of 1us
	timer_startTimer(micros - 1);

	while(!(TIMER0_RIS_R & TIMER_RIS_TATORIS));
	TIMER0_ICR_R |= TIMER_RIS_TATORIS;

	timer_stopTimer();
}

void timer_startTimer(uint16_t startValue) {
	//Enable Wide Timer0
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;

	//Disable timer (clear bit TnEN in GPTMCTL)
	TIMER0_CTL_R &= ~TIMER_CTL_TAEN;

	//Set as 16-bit timer
	TIMER0_CFG_R = TIMER_CFG_16_BIT;

	//Configure the timer for periodic mode
	//and countdown
	TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;

	//Load initial timer value
	TIMER0_TAILR_R = startValue;

	//Clear the timout flag
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;

	//Enable Timer0A time out interrupt
	TIMER0_IMR_R = TIMER_IMR_TATOIM;

	//Set the prescaler to 16 (period = 1us)
	TIMER0_TAPR_R = 16;

	//Clear the interrupt mask register
	TIMER0_IMR_R = 0;

	//Enable Timer0 A
	TIMER0_CTL_R |= TIMER_CTL_TAEN;

	//Do some stuff with NVIC???
}

void timer_stopTimer(void) {
	//Disable timer (clear bit TnEN in GPTMCTL)
	TIMER0_CTL_R &= ~TIMER_CTL_TAEN;

	//Disable system control for Timer0
	SYSCTL_RCGCTIMER_R &= ~SYSCTL_RCGCTIMER_R0;
}
