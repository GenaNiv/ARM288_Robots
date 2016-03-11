/*
 * main.c
 */
#include <inc/tm4c123gh6pm.h>
#include "util.h"
#include "lcd.h"
#include <stdint.h>


//Select ther correct header
#define VERSION1	//Remove to use IRobot Create 2 Platform

#ifdef VERSION1
	#include "open_interface_v1.h"
#else
	#include "open_interface.h"
#endif



int main(void) {

	const char theCat[] = 	"  /\\_/\\ 	\r\n"
							"=( °w° )=	\r\n"
							"  )   (  // \r\n"
							" (__ __)// \r\n\r\n";
	lcd_init();

	oi_t * theRobot = oi_alloc();

	oi_init(theRobot);

	while(1)
	{
		oi_update(theRobot);
		//uart_sendBuff((uint8_t *)theCat, strlen(theCat));
		//oi_uartSendChar('A');
		timer_waitMillis(1000);

		oi_setWheels(100, 100);

		timer_waitMillis(1000);

		oi_setWheels(500, 500);

	}


	lcd_puts("HELLO WORLD");
	timer_waitMillis(1000);
	lcd_gotoLine(1);
	lcd_puts("HELLO WORLD");
	timer_waitMillis(1000);
	lcd_gotoLine(2);
	lcd_puts("HELLO WORLD");
//a;dlkfj;alkdfj
	//Git Ignore Test
	timer_waitMillis(1000);
	lcd_gotoLine(4);
	lcd_puts("HELLO WORLD");

	timer_waitMillis(1000);

	while(1);
	return 0;
}
