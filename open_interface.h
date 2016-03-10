/*
 * Open Interface
 *
 *  Created on: Mar 3, 2016
 *      Author: Noah Bergman
 */

#ifndef OPEN_INTERFACE_H_
#define OPEN_INTERFACE_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>

///Initialize UART3 for OI Communication and Debugging
void uart_init(void);

///transmit character
void uart_sendChar(char data);

///transmit character array
void uart_sendStr(const char *theData);

///Receive from UART
char uart_receive(void);

///Send large data set from array
void uart_sendBuff(const uint8_t theData[], uint8_t theSize);


#endif /* UARTCONFIG_H_ */
