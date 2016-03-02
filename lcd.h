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

/// Initialize PORT___ to communicate with LCD
void lcd_init(void);

///Toggle Enable pin and clear data on port
void lcd_sendChar(char data);

void lcd_sendCommand(uint8_t data);

///Send 4bit nibble to lcd, then clear port.
void lcd_sendNibble(uint8_t theNibble);

///Clear LCD Screen
void lcd_clear(void);

#endif /* LCD_H_ */
