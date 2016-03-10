/*
 * main.c
 */
#include <inc/tm4c123gh6pm.h>
#include "util.h"
#include "lcd.h"
#include <stdint.h>
#include "open_interface.h"


int main(void) {

	const char theCat[] = 	"  /\\_/\\ 	\r\n"
							"=( °w° )=	\r\n"
							"  )   (  // \r\n"
							" (__ __)// \r\n\r\n";
	lcd_init();
	uart_init();

	while(1)
	{
		//uart_sendBuff((uint8_t *)theCat, strlen(theCat));
		uart_sendChar('A');
		timer_waitMillis(500);
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
