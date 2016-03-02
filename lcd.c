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



///TODO Find pinout of LCD
#define EN_PIN  BIT6
#define RS_PIN		BIT4
#define RW_PIN		BIT5
#define LCD_PORT_DATA	GPIO_PORTB_DATA_R


//TODO: Poll Busy Flag

/// Initialize PORT___ to communicate with LCD
void lcd_init(void)
{
	volatile uint32_t i = 0;
	SYSCTL_RCGCGPIO_R = BIT1; //Turn on PORTB Sys Clk

	//Set port to output
	GPIO_PORTB_DIR_R |= 0x7F; //Pins 0 - 6
	GPIO_PORTB_DEN_R |= 0x7F;

	LCD_PORT_DATA &= ~(EN_PIN | RW_PIN | RS_PIN);

	//Delay 40msec after power applied
	for(i = 0; i < 1000000; i++);

	//Wake up
	lcd_sendNibble(0x03);
	for(i = 0; i < 1000000; i++);
	lcd_sendNibble(0x03);
	for(i = 0; i < 1000000; i++);
	lcd_sendNibble(0x03);
	for(i = 0; i < 1000000; i++);


	lcd_sendNibble(0x02);			//Function set 4 bit
	for(i = 0; i < 100000; i++);

	lcd_sendCommand(0x28);			//Function 4 bit / 2 lines
	for(i = 0; i < 100000; i++);

	lcd_sendCommand(0x10);			//Set cursor
	for(i = 0; i < 10000; i++);

	//lcd_sendCommand(HD_BLINK_ON | HD_CURSOR_ON | HD_DISPLAY_ON);
	lcd_sendCommand(0x0F);
	for( i = 0; i < 10000; i++);

	lcd_sendCommand(0x28);			//Function 4 bit / 2 lines
	for(i = 0; i < 10000; i++);


	lcd_sendCommand(0x06);			//Increment Cursor / No Display Shift
	for(i = 0; i < 10000; i++);


	lcd_sendCommand(0x01);			//Return Home
	for(i = 0; i < 1000000; i++);


	//lcd_clear();
	lcd_sendCommand(0x02);
	for(i = 0; i < 1000000; i++);

	//lcd_sendChar('a');

	//TODO: wait_ms(5)



	//Toggle enable
}

///Toggle Enable pin and clear data on port
void lcd_sendChar(char data)
{
	//Select - Send Data
	LCD_PORT_DATA |= RS_PIN;
	LCD_PORT_DATA &= ~(RW_PIN);

	//Send High nibble
	lcd_sendNibble(data >> 4);

	//TODO: wait_ms(1);
	volatile uint32_t i = 0;
	for(i = 0; i < 10000; i++);

	//Send Lower Nibble
	lcd_sendNibble(data & 0x0F);

}


///Send Command to LCD Controller
void lcd_sendCommand(uint8_t data)
{
	//Enable High
	LCD_PORT_DATA |= EN_PIN;
	LCD_PORT_DATA &= ~(RW_PIN | RS_PIN); // Write Command

	//Send High nibble
	lcd_sendNibble(data >> 4);

	//TODO: wait_ms(1);
	int i = 0;
	for(i = 0; i < 10000; i++);

	//Send Lower Nibble
	lcd_sendNibble(data & 0x0F);


}

///Send 4bit nibble to lcd, then clear port.
void lcd_sendNibble(uint8_t theNibble)
{
	int i = 0;
	LCD_PORT_DATA |= EN_PIN;
	LCD_PORT_DATA |= (theNibble & 0x0F); //PortB0 - 3
	//wait 40 ns;

	for(i = 0; i < 10000; i++);
	//Clock in Data
	LCD_PORT_DATA &= ~(EN_PIN);

	//TODO: wait_ms(1);

	for(i = 0; i < 10000; i++);

	//Clear Port
	LCD_PORT_DATA &= ~(0x0F);


}

///Clear LCD Screen
void lcd_clear(void)
{
	lcd_sendCommand(HD_LCD_CLEAR);

	//wait_ms(1);

}
