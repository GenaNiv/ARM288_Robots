/*
 * Open Interface
 *
 *  Created on: Mar 3, 2016
 *      Author: mpost7
 */
#include "open_interface.h"

void uart_init(void)
{
	//TODO: Calculate this from baudRate
	uint16_t iBRD = 0x08;
	uint16_t fBRD = 0x2C;

	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2; //enable GPIO Port C

	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R3; //enable UART3

	GPIO_PORTC_AFSEL_R |= (BIT6 | BIT7); //Enable alternate function on PC6,PC7
	GPIO_PORTC_PCTL_R |= 0x11000000; //Enable function 1 (UART Rx/Tx) on PC6,PC7
	GPIO_PORTC_DEN_R |= (BIT6 | BIT7); //Enable PC6,PC7 output
	GPIO_PORTC_DIR_R |= BIT7; //Set pin direction to output on PC7
	//GPIO_PORTC_DIR_R &= ~BIT7; //Set pin direction to input on PC6

	UART3_CTL_R &= ~(UART_CTL_UARTEN); //Disable UART3 while we mess with it

	UART3_IBRD_R = iBRD;
	UART3_FBRD_R = fBRD;

	UART3_LCRH_R = UART_LCRH_WLEN_8; //8 bit, 1 stop, no parity, no FIFO
	UART3_CC_R = UART_CC_CS_SYSCLK; //Use System Clock
	UART3_CTL_R = UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN; //Enable Rx, Tx and UART module
}

void uart_sendChar(char data)
{
	while((UART3_FR_R & UART_FR_TXFF) != 0); //holds until no data in transmit buffer
	UART3_DR_R = data; //puts data in transmission buffer
}

char uart_receive(void)
{
	//uint32_t tempData; //used for error checking
	char data;

	while((UART3_FR_R & UART_FR_RXFE)); //wait here until data is recieved

	data = (char)(UART3_DR_R & 0xFF);

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
