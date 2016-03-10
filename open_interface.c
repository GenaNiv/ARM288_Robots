/*
 * Open Interface
 *
 *  Created on: Mar 3, 2016
 *      Author: mpost7
 */
#include "open_interface.h"

void uart_init(void)
{
	SYSCTL_RCGCGPIO_R |= 0x03; //enable GPIO Port c
	SYSCTL_RCGCUART_R = 0x04; //enable and provide clock for UART mod 3

	GPIO_PORTC_AFSEL_R = 0xC0; // set PC7:6 in AFSEL for UART3 Tx and Rx
	GPIO_PORTC_PCTL_R = 0x11000000; //AFSEL has bit 6 and 7 set so we need nibble 6 and 7 set in PCTL for PB1:0
	GPIO_PORTC_DEN_R = 0xC0; //enable PC7:6

	UART3_CTL_R &= 0xFFFFFFFE; //disable uart 3 by clearing EN in CTL register
	UART3_IBRD_R = 0x08; //BRD=SYSCLK/((ClkDiv)(BaudRate)), HSE=0 ClkDiv=16, BaudRate=115,200
	UART3_FBRD_R = 0x2C; //Fractional remainder is 0.6805, DIVFRAC = (.6805)(64)+0.5 = 44
	UART3_LCRH_R |= 0x60; //8 bit, 1 stop, no parity, FIFO's diasabled, 0x70 enables FIFO's
	UART3_CC_R = 0x0; //set SysClk as source
	UART3_CTL_R |= 0x01; //Enable UART3
}

void uart_sendChar(char data)
{
	while((UART3_FR_R & 0x20) != 0); //holds until no data in transmit buffer
	UART3_DR_R = data; //puts data in transmission buffer
}

char uart_receive(void)
{
	//uint32_t tempData; //used for error checking
	char data;

	while((UART3_FR_R & 0x10) != 0); //wait here until data is recieved
	data = UART3_DR_R;
	//ToDo: Implement error checking
	return data;
}


///transmit character array
void uart_sendStr(const char *theData)
{
	while(*theData != '\0')
	{
		uart_sendChar(*theData);
		theData++;
	}

}

void uart_sendBuff(const uint8_t theData[], uint8_t theSize)
{
	int i;

	for(i = 0; i < theSize; i++)
	{
		uart_sendChar(theData[i]);
	}
}
