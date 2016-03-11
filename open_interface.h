/*
 * open_interface.h
 *
 * Contains all functionality to interface with the IRobot Create V2
 * Communication over UART3 at 115200
 *
 * @author Noah Bergman
 * @date 03/11/2016
 *
 *
 *
 */

#ifndef OPEN_INTERFACE_H_
#define OPEN_INTERFACE_H_



#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>


///	\brief Initialize UART3 for OI Communication at 115200
///	internal function
void oi_uartInit(void);


///transmit character
///	internal function
void oi_uartSendChar(char data);

///transmit character array
///	internal function
void uart_sendStr(const char *theData);

///Receive from UART
///	internal function
char oi_uartReceive(void);

///Send large data set from array
///	internal function
void oi_uartSendBuff(const uint8_t theData[], uint8_t theSize);




#endif /* OPEN_INTERFACE_H_ */
