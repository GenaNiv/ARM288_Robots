/**
 * lcd.c: Functions for displaying content on the 4x16 Character LCD Screen
 *
 *  @author Noah Bergman
 *  @date 02/29/2016
 *
 *
 */




#include "lcd.h"

#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80


//Defines for LCD Control Commands
#define HD_LCD_CLEAR 		0x01
#define HD_RETURN_HOME		0X02

#define HD_CURSOR_SHIFT_DEC	0X05
#define HD_CURSOR_SHIFT_INC	0X07
#define HD_DISPLAY_CONTROL	3
#define HD_DISPLAY_ON 		0x04
#define HD_CURSOR_ON		0x02
#define HD_BLINK_ON			0x01
#define HD_CURSOR_MOVE_LEFT 0x10
#define HD_CURSOR_MOVE_RIGHT 0x14
#define HD_DISPLAY_SHIFT_LEFT 0x18
#define HD_DISPLAY_SHIFT_RIGHT 0x1C

#define LCD_WIDTH 20
#define LCD_HEIGHT 4
#define LCD_TOTAL_CHARS (LCD_WIDTH * LCD_HEIGHT)
#define LCD_DDRAM_WRITE 0x80
#define LCD_CGRAM_WRITE 0x40

///TODO Find pinout of LCD
#define EN_PIN  	BIT6
#define RS_PIN		BIT4
#define RW_PIN		BIT5
#define LCD_PORT_DATA	GPIO_PORTB_DATA_R


//TODO: Poll Busy Flag

/// Initialize PORTB0:6 to Communicate with LCD
void lcd_init(void)
{
	//TODO: Remove waitMillis after commands -- poll busy flag in sendCommand
	volatile uint32_t i = 0;
	SYSCTL_RCGCGPIO_R = BIT1; //Turn on PORTB Sys Clk

	//Set port to output
	GPIO_PORTB_DIR_R |= 0x7F; //Pins 0 - 6
	GPIO_PORTB_DEN_R |= 0x7F;

	LCD_PORT_DATA &= ~(EN_PIN | RW_PIN | RS_PIN);

	//Delay 40msec after power applied
	timer_waitMillis(40);

	//Wake up
	lcd_sendNibble(0x03);
	timer_waitMillis(5);

	lcd_sendNibble(0x03);
	timer_waitMicros(160);

	lcd_sendNibble(0x03);
	timer_waitMicros(160);

	lcd_sendNibble(0x02);			//Function set 4 bit
	timer_waitMillis(1);

	lcd_sendCommand(0x28);			//Function 4 bit / 2 lines
	timer_waitMillis(1);

	lcd_sendCommand(0x10);			//Set cursor
	timer_waitMillis(1);

	//lcd_sendCommand(HD_BLINK_ON | HD_CURSOR_ON | HD_DISPLAY_ON);
	lcd_sendCommand(0x0F);
	timer_waitMillis(1);

	lcd_sendCommand(0x28);			//Function 4 bit / 2 lines
	timer_waitMillis(1);


	lcd_sendCommand(0x06);			//Increment Cursor / No Display Shift
	timer_waitMillis(1);


	lcd_sendCommand(0x01);			//Return Home
	timer_waitMillis(1);

	lcd_clear();
	timer_waitMillis(1);

}

///Send Char to LCD
void lcd_putc(char data)
{
	//Select - Send Data
	LCD_PORT_DATA |= RS_PIN;
	LCD_PORT_DATA &= ~(RW_PIN);

	//Send High nibble
	lcd_sendNibble(data >> 4);

	timer_waitMicros(40);

	//Send Lower Nibble
	lcd_sendNibble(data & 0x0F);

	//TODO: Poll Busy flag
}

///Send Character array to LCD
void lcd_puts(char data[])
{
	//While not equal to null
	while(*data != '\0')
	{
		lcd_putc(*data);
		data++;
	}
}

///Send Command to LCD - Position, Clear, Etc.
void lcd_sendCommand(uint8_t data)
{
	//Enable High
	LCD_PORT_DATA |= EN_PIN;
	LCD_PORT_DATA &= ~(RW_PIN | RS_PIN); // Write Command

	//Send High nibble
	lcd_sendNibble(data >> 4);

	timer_waitMicros(1);
	//Send Lower Nibble
	lcd_sendNibble(data & 0x0F);

	//TODO: Poll Busy Flag

}


///Send 4bit nibble to lcd, then clear port.
void lcd_sendNibble(uint8_t theNibble)
{
	LCD_PORT_DATA |= EN_PIN;
	LCD_PORT_DATA |= (theNibble & 0x0F); //PortB0 - 3

	//Data Hold time before Clock = 40ns -- Change if faster clock
	timer_waitMicros(10);
	//Clock in Data
	LCD_PORT_DATA &= ~(EN_PIN);

	timer_waitMicros(10);
	//Clear Port
	LCD_PORT_DATA &= ~(0x0F);


}

///Clear LCD Screen
void inline lcd_clear(void)
{
	lcd_sendCommand(HD_LCD_CLEAR);

}

///Return Cursor to 0,0
void inline lcd_home(void)
{
	lcd_sendCommand(HD_RETURN_HOME);
}

///Goto 0 indexed line number
void lcd_gotoLine(uint8_t lineNum)
{

	//Address of the four line elements
	static const uint8_t lineAddress[] = {0x00, 0x40, 0x14, 0x54};

	lineNum = (0x03 & lineNum); // Mask input for 0 - 3
	lcd_sendCommand(LCD_DDRAM_WRITE | lineAddress[lineNum]);

}

/// Print a formatted string to the LCD screen
/**
 * Mimics the C library function printf for writing to the LCD screen.  The function is buffered; i.e. if you call
 * lprintf twice with the same string, it will only update the LCD the first time.
 *
 * Google "printf" for documentation on the formatter string.
 *
 * Code from this site was also used: http://www.ozzu.com/cpp-tutorials/tutorial-writing-custom-printf-wrapper-function-t89166.html
 * @author Kerrick Staley & Chad Nelson
 * @date 05/16/2012
 */
void lprintf(const char *format, ...) {
	static char lastbuffer[LCD_TOTAL_CHARS + 1];

	char buffer[LCD_TOTAL_CHARS + 1];
	va_list arglist;
	va_start(arglist, format);
	vsnprintf(buffer, LCD_TOTAL_CHARS + 1, format, arglist);

	if (!strcmp(lastbuffer, buffer))
		return;

	strcpy(lastbuffer, buffer);
	lcd_clear();
	char *str = buffer;
	int charnum = 0;
	while (*str && charnum < LCD_TOTAL_CHARS) {
		if (*str == '\n') {
			/* fill remainder of line with spaces */
			charnum += LCD_WIDTH - charnum % LCD_WIDTH;
		} else {
			lcd_putc(*str);
			charnum++;
		}

		str++;

		/*
		 * The LCD's lines are not sequential; for future reference, the address are like
		 * 0x00...0x13 : line 1
		 * 0x14...0x27 : line 3
		 * 0x28...0x3F : random junk
		 * 0x40...0x53 : line 2
		 * 0x54...0x68 : line 4
		 *
		 * The cursor position must be reset at the end of every line, otherwise, after writing line 1, it writes line 3 and then nothingness
		 */

		if (charnum % LCD_WIDTH == 0) {
			switch (charnum / LCD_WIDTH) {
			case 1:
				lcd_gotoLine(2);
				break;
			case 2:
				lcd_gotoLine(3);
				break;
			case 3:
				lcd_gotoLine(4);
			}
		}
	}
	va_end(arglist);
}

