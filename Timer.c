/*
 * Timer.cpp
 *
 *  @author Eric Middleton
 */

#include "Timer.h"

volatile uint32_t _timer_ticks;

void timer_waitMillis(uint32_t millis) {
	///Start timer with period of 1ms
	timer_startTimer(999);

	///loop until enough milliseconds have passed
	while(millis > 0) {
		///wait until the millisecond timer has timed out
		while(!(TIMER0_RIS_R & TIMER_RIS_TATORIS));

		///Reset the timeout flag
		TIMER0_ICR_R |= TIMER_RIS_TATORIS;

		millis--;
	}

	///Stop the timer
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
	//TIMER0_IMR_R = TIMER_IMR_TATOIM;

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
