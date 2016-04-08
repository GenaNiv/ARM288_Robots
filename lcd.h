/*
 * lcd.h
 *
 *  Created on: Mar 1, 2016
 *      Author: nbergman
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "util.h"
#include "timer.h"

/// Initialize PORTB0:6 to Communicate with LCD
void lcd_init(void);

///Send Char to LCD
void lcd_putc(char data);

///Send Character array to LCD
void lcd_puts(char data[]);

///Send Command to LCD - Position, Clear, Etc.
void lcd_sendCommand(uint8_t data);

///Send 4bit nibble to lcd, then clear port.
void lcd_sendNibble(uint8_t theNibble);

///Clear LCD Screen
void inline lcd_clear(void);

///Return Cursor to 0,0
void inline lcd_home(void);

///Goto Line on LCD - 0 Indexed
void lcd_gotoLine(uint8_t lineNum);

void lprintf(const char *format, ...);

#endif /* LCD_H_ */
