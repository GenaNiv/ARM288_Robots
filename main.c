/*
 * main.c
 */
#include <inc/tm4c123gh6pm.h>
#include "util.h"
#include "lcd.h"
#include <stdint.h>
#include "ping.h"
#include "Timer.h"
#include "servo.h"


//Select ther correct header
#define VERSION1	//Remove to use IRobot Create 2 Platform

#ifdef VERSION1
	#include "open_interface_v1.h"
#else
	#include "open_interface.h"
#endif



int main(void) {
	//lcd_init();

//	oi_t * theRobot = oi_alloc();
//	oi_init(theRobot);
/*
	SYSCTL_RCGCGPIO_R |= BIT5; //turn on clk to PORTF
	GPIO_PORTF_AFSEL_R = 0;
	GPIO_PORTF_PCTL_R = 0;
	GPIO_PORTF_DIR_R |= BIT1; //set for output
	GPIO_PORTF_DEN_R |= BIT1;

	while(1) {
		GPIO_PORTF_DATA_R ^= BIT1;
		timer_waitMillis(1000);
	}
*/
	Servo_Test();

	return 0;
}
