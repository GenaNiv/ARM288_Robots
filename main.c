/*
 * main.c
 */
#include <inc/tm4c123gh6pm.h>
#include "util.h"
#include "lcd.h"
#include <stdint.h>

int main(void) {
	//a;lad;sadfgsdfglkajdf;lkajfsd
	lcd_init();
	lcd_sendChar('H');
	lcd_sendChar('E');
	lcd_sendChar('L');
	lcd_sendChar('L');
	lcd_sendChar('O');
	lcd_sendChar(' ');
	lcd_sendChar('W');
	lcd_sendChar('O');
	lcd_sendChar('R');
	lcd_sendChar('L');
	lcd_sendChar('D');


	while(1);
	return 0;
}
