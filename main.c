/*
 * main.c
 */
#include <inc/tm4c123gh6pm.h>
#include "util.h"
#include "lcd.h"
#include <stdint.h>

int main(void) {

	lcd_init();

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
