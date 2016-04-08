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



	/*
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

		oi_setWheels(500, 500);_

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
	*/
/*
int counter =0;
float pingDistance = 0;
	while(1)
	{
		Ping_Init();
		Ping_TimCapInit();
		timer_waitMillis(100);
		pingDistance = Ping_getDistance();
		timer_waitMillis(10);
	}
*/
	Servo_Init();
	while(1)
	{

		Servo_Move(0);
		//Servo_Scan();
		timer_waitMillis(1000);

	}
	return 0;
}
