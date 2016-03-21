/*
 * open_interface.c
 *
 *  Created on: Mar 11, 2016
 *      Author: nbergman
 */


/*
 * Open Interface
 *
 *  Created on: Mar 3, 2016
 *      Author: Noah Bergman, Eric Middleton
 */
#include "open_interface.h"


///Allocate and clear all memory for OI Struct
oi_t* oi_alloc()
{
	return calloc(1, sizeof(oi_t));
}


///Free memory from pointer to Open Interface Struct
void oi_free(oi_t *self)
{
	free(self);
}


/**
 * Initialize open interface communication with IRobot.
 *
 * This function needs to be called to setup UART and other
 * OI peripherals
 *
 *
 */
void oi_init(oi_t *self)
{
	oi_uartInit();
	oi_uartSendChar(OI_OPCODE_START);

	//oi_uartSendChar(OI_OPCODE_BAUD);
	//oi_uartSendChar(0x08);		//Baud code for 28800

	//timer_waitMillis(100);
	//oi_uartFastMode();			//Set baud to 28800

	oi_uartSendChar(OI_OPCODE_FULL);		//Use full mode, unrestricted control
	oi_setLeds(1,1,7,255);

	oi_update(self);
	oi_update(self); //Call twice to clear distance/angle

}


///Update all sensor and store in oi_t struct
void oi_update(oi_t *self)
{
	int i =0;
	//Clear RX Buffer
	//int i = 0;
	//while(RX FLAG) i = RX_DATA

	//Query list of sensors
	oi_uartSendChar(OI_OPCODE_SENSORS);
	oi_uartSendChar(OI_SENSOR_PACKET_GROUP100);

	//TODO: For Create2, packet size 80, group 100

	// Read all the sensor data
	char *sensor = (char *) self;
	for (i = 0; i < 80; i++) {
		// read each sensor byte
		*(sensor++) = oi_uartReceive();
	}

	sensor = (char *) self;

	// Fix byte ordering for multi-byte members of the struct
	self->distance                       = (sensor[12] << 8) + sensor[13];
		self->angle                          = (sensor[14] << 8) + sensor[15];
		self->voltage                        = (sensor[17] << 8) + sensor[18];
		self->current                        = (sensor[19] << 8) + sensor[20];
		self->charge                         = (sensor[22] << 8) + sensor[23];
		self->capacity                       = (sensor[24] << 8) + sensor[25];
		self->wall_signal                    = (sensor[26] << 8) + sensor[27];
		self->cliff_left_signal              = (sensor[28] << 8) + sensor[29];
		self->cliff_frontleft_signal         = (sensor[30] << 8) + sensor[31];
		self->cliff_frontright_signal        = (sensor[32] << 8) + sensor[33];
		self->cliff_right_signal             = (sensor[34] << 8) + sensor[35];
		self->cargo_bay_voltage              = (sensor[41] << 8) + sensor[42];
		self->requested_velocity             = (sensor[48] << 8) + sensor[42];
		self->requested_radius               = (sensor[50] << 8) + sensor[51];
		self->requested_right_velocity       = (sensor[52] << 8) + sensor[53];
		self->requested_left_velocity        = (sensor[54] << 8) + sensor[55];
		//Icreate2
		self->left_encoder_counts            = (sensor[56] << 8) + sensor[57];
		self->right_encoder_counts           = (sensor[58] << 8) + sensor[59];
		self->light_bump_left_signal         = (sensor[61] << 8) + sensor[62];
		self->light_bump_front_left_signal   = (sensor[63] << 8) + sensor[64];
		self->light_bump_center_left_signal  = (sensor[65] << 8) + sensor[66];
		self->light_bump_center_right_signal = (sensor[67] << 8) + sensor[68];
		self->light_bump_front_right_signal  = (sensor[69] << 8) + sensor[70];
		self->light_bump_right_signal        = (sensor[71] << 8) + sensor[72];
		//left char = 73
		//right char = 74
		self->left_motor_current             = (sensor[75] << 8) + sensor[76];
		self->right_motor_current            = (sensor[77] << 8) + sensor[78];
		self->main_brush_motor_current       = (sensor[79] << 8) + sensor[80];
		self->side_brush_motor_current       = (sensor[81] << 8) + sensor[82];


		timer_waitMillis(25); // reduces USART errors that occur when continuously transmitting/receiving min wait time=15ms
}

/// \brief Set the LEDS on the Create
/// \param play_led 0=off, 1=on
/// \param advance_led 0=off, 1=on
/// \param power_color (0-255), 0=green, 255=red
/// \param power_intensity (0-255) 0=off, 255=full intensity
void oi_setLeds(uint8_t play_led, uint8_t advance_led, uint8_t power_color, uint8_t power_intensity)
{
	// LED Opcode
	oi_uartSendChar(OI_OPCODE_LEDS);

	// Set the Play and Advance LEDs
	oi_uartSendChar(advance_led << 3 && play_led << 2);

	// Set the power led color
	oi_uartSendChar(power_color);

	// Set the power led intensity
	oi_uartSendChar(power_intensity);
}

/// \brief Set direction and speed of the robot's wheels
/// \param linear velocity in mm/s values range from -500 -> 500 of right wheel
/// \param linear velocity in mm/s values range from -500 -> 500 of left wheel
void oi_setWheels(int16_t right_wheel, int16_t left_wheel)
{
	oi_uartSendChar(OI_OPCODE_DRIVE_WHEELS);
	oi_uartSendChar(right_wheel>>8);
	oi_uartSendChar(right_wheel & 0xff);
	oi_uartSendChar(left_wheel>>8);
	oi_uartSendChar(left_wheel& 0xff);
}


/// \brief Load song sequence
/// \param An integer value from 0 - 15 that acts as a label for note sequence
/// \param An integer value from 1 - 16 indicating the number of notes in the sequence
/// \param A pointer to a sequence of notes stored as integer values
/// \param A pointer to a sequence of durations that correspond to the notes
void oi_loadSong(int song_index, int num_notes, unsigned char  *notes, unsigned char  *duration)
{
	int i;
	oi_uartSendChar(OI_OPCODE_SONG);
	oi_uartSendChar(song_index);
	oi_uartSendChar(num_notes);
	for (i=0;i<num_notes;i++) {
		oi_uartSendChar(notes[i]);
		oi_uartSendChar(duration[i]);
	}
}

/// Plays a given song; use oi_load_song(...) first
void oi_play_song(int index){
	oi_uartSendChar(OI_OPCODE_PLAY);
	oi_uartSendChar(index);
}


/// Runs default go charge program; robot will search for dock
void go_charge(void) {
	char charging_state=0;

/*	//Calling demo that will cause Create to seek out home base
	oi_uartSendChar(OI_OPCODE_MAX);
	oi_uartSendChar(0x01);

	//Control is returned immediately, so need to check for docking status
	DDRB &= ~0x80; //Setting pin7 to input
	PORTB |= 0x80; //Setting pullup on pin7

	do {
		charging_state = PINB >> 7;
	} while (charging_state == 0);
	*/
}




///	\brief Initialize UART3 for OI Communication and Debugging
///	internal function
void oi_uartInit(void)
{
	//Calculated Baudrate for 115200;
	uint16_t iBRD = 0x08;//BRD=SYSCLK/((ClkDiv)(BaudRate)), HSE=0 ClkDiv=16, BaudRate=115,200
	uint16_t fBRD = 46;//Fractional remainder is 0.6805, DIVFRAC = (.6805)(64)+0.5 = 44

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

/*
 * This already is default baud rate
///Set baud to 115200
void oi_uartFastMode(void)
{
	UART3_CTL_R &= ~(UART_CTL_UARTEN); //Disable UART3 while we mess with it

//	UART3_IBRD_R = 0x08; //BRD=SYSCLK/((ClkDiv)(BaudRate)), HSE=0 ClkDiv=16, BaudRate=115,200
//	UART3_FBRD_R = 0x2C; //Fractional remainder is 0.6805, DIVFRAC = (.6805)(64)+0.5 = 44

	//Dividers for baud=28800
	UART3_IBRD_R = 34;
	UART3_FBRD_R = 46;

	UART3_CTL_R |= UART_CTL_UARTEN;
}
*/

///transmit character
///	internal function
void oi_uartSendChar(char data)
{
	while((UART3_FR_R & UART_FR_TXFF) != 0); //holds until no data in transmit buffer
	UART3_DR_R = data; //puts data in transmission buffer
}

char oi_uartReceive(void)
{
	//uint32_t tempData; //used for error checking
	char data;

	while((UART3_FR_R & UART_FR_RXFE)); //wait here until data is recieved

	data = (char)(UART3_DR_R & 0xFF);

	//ToDo: Implement error checking
	return data;
}


///transmit character array
void oi_uartSendStr(const char *theData)
{
	while(*theData != '\0')
	{
		oi_uartSendChar(*theData);
		theData++;
	}

}

void oi_uartSendBuff(const uint8_t theData[], uint8_t theSize)
{
	int i;

	for(i = 0; i < theSize; i++)
	{
		oi_uartSendChar(theData[i]);
	}
}
