/**
 * util.c
 *
 * Functions to be used on the TIVA C Series Microcontroller
 *
 *
 *
 * @author Noah Bergman
 * @date 02/29/2016
 */

#include <stdio.h>
#include <stdint.h>
#include "util.h"
#include <inc\tm4c123gh6pm.h>

//Global Variable used for timer interrupts
volatile unsigned int timer2_tick;

///Blocks for specified number of milliseconds
void wait_ms(unsigned int timer_val)
{
	//TODO: Set Time prescalar
	timer2_tick = 0;
	//Timer Start

	while(timer2_tick < timer_val);

	//Timer Stop

}

void timer_start()
{
	//TODO Timer Setup

	//Two options for delays ms / us

	//Enable interupts
}

void timer_stop()
{
	//TODO Stop Timer
}


//TODO: Timer ISR

