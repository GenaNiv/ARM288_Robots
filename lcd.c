/**
 * lcd.c: Functions for displaying content on the 4x16 Character LCD Screen
 *
 *  @author Noah Bergman
 *  @date 02/29/2016
 *
 *
 */



#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "util.h"
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
#define HD_DISPLAY_ON 		2
#define HD_CURSOR_ON		1
#define HD_BLINK_ON			0
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



/// Initialize PORT___ to communicate with LCD
void lcd_init(void)
{
	SYSCTL_RCGCGPIO_R = BIT1; //Turn on PORTB Sys Clk

	//Set port to output
	GPIO_PORTB_DIR_R |= 0x7F; //Pins 0 - 6
	GPIO_PORTB_DEN_R |= 0x7F;

	LCD_PORT_DATA &= ~(EN_PIN);

	//Delay 40msec after power applied

	lcd_command(0x03);
	//TODO: wait_ms(5)



	//Toggle enable
}

///Toggle Enable pin and clear data on port
void lcd_sendChar(char data)
{
	//Send Data
	LCD_PORT_DATA |= RS_PIN;
	LCD_PORTA_DATA &= ~(RW_PIN | EN_PIN);

	//Send High nibble
	lcd_sendNibble(data >> 4);

	//Send Lower Nibble
	lcd_sendNibble(data & 0x0F);


}

///Send 4bit nibble to lcd
void lcd_sendNibble(uint8_t theNibble)
{

	LCD_PORT_DATA |= theNibble;


}


///Send Command to LCD Controller
void lcd_command(char data)
{
	LCD_PORT_DATA |= (data & 0x0F); //Lower Nibble
	LCD_PORT_DATA &= ~(RW_PIN | RS_PIN); // Write Command

	//Enable High
	LCD_PORT_DATA |= EN_PIN;

	//TODO: wait_ms(1);
	for(int i = 0; i < 100; i++);

	LCD_PORT_DATA &= ~(EN_PIN);


}

///
void lcd_clear(void)
{
	lcd_command(HD_LCD_CLEAR);
	wait_ms(1);

}
