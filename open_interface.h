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



#define OI_OPCODE_START            128
#define OI_OPCODE_BAUD             129
#define OI_OPCODE_CONTROL          130
#define OI_OPCODE_SAFE             131
#define OI_OPCODE_FULL             132
#define OI_OPCODE_POWER            133		//POWER DOWN ROOMBA
#define OI_OPCODE_SPOT             134
#define OI_OPCODE_CLEAN            135
#define OI_OPCODE_MAX              136
#define OI_OPCODE_DRIVE            137
#define OI_OPCODE_MOTORS           138
#define OI_OPCODE_LEDS             139
#define OI_OPCODE_SONG             140
#define OI_OPCODE_PLAY             141
#define OI_OPCODE_SENSORS          142
#define OI_OPCODE_FORCEDOCK        143

#define OI_OPCODE_PWM_MOTORS       144
#define OI_OPCODE_DRIVE_WHEELS     145
#define OI_OPCODE_DRIVE_PWM        146
#define OI_OPCODE_OUTPUTS          147
#define OI_OPCODE_STREAM           148
#define OI_OPCODE_QUERY_LIST       149
#define OI_OPCODE_DO_STREAM        150
#define OI_OPCODE_SEND_IR_CHAR     151
#define OI_OPCODE_SCRIPT           152
#define OI_OPCODE_PLAY_SCRIPT      153
#define OI_OPCODE_SHOW_SCRIPT      154
#define OI_OPCODE_WAIT_TIME        155
#define OI_OPCODE_WAIT_DISTANCE    156
#define OI_OPCODE_WAIT_ANGLE       157
#define OI_OPCODE_WAIT_EVENT       158

#define OI_OPCODE_RESET				7	//RESET ROOMBA - GO TO
#define OI_OPCODE_STOP				173
#define OI_OPCODE_SCHEDULE			167
#define OI_OPCODE_SCHED_LED			162	//MONDAY THROUGH SUNDAY LEDS
#define OI_OPCODE_7SEG				163


// Contains Packets 7-26
#define OI_SENSOR_PACKET_GROUP0 0
// Contains Packets 7-16
#define OI_SENSOR_PACKET_GROUP1 1
// Contains Packets 17-20
#define OI_SENSOR_PACKET_GROUP2 2
// Contains Packets 21-26
#define OI_SENSOR_PACKET_GROUP3 3
// Contains Packets 27-34
#define OI_SENSOR_PACKET_GROUP4 4
// Contains Packets 35-42
#define OI_SENSOR_PACKET_GROUP5 5
// Contains Packets 7-42
#define OI_SENSOR_PACKET_GROUP6 6



/// iRobot Create Sensor Data
//TODO: Double Check for Irobot 2
typedef struct {
	// Sensor statuses (booleans)
	uint8_t bumper_right : 1;
	uint8_t bumper_left : 1;
	uint8_t wheeldrop_right : 1;
	uint8_t wheeldrop_left : 1;
	uint8_t wheeldrop_caster : 1;
	uint8_t wall; // not virtual wall
	uint8_t cliff_left;
	uint8_t cliff_frontleft;
	uint8_t cliff_frontright;
	uint8_t cliff_right;
	uint8_t virtual_wall; // omni-directional IR sensor

	// Over current information
	uint8_t overcurrent_ld1 : 1;
	uint8_t overcurrent_ld0 : 1;
	uint8_t overcurrent_ld2 : 1;
	uint8_t overcurrent_driveright : 1;
	uint8_t overcurrent_driveleft : 1;

	uint16_t unused_bytes;

	uint8_t infrared_byte;
	uint8_t button_play : 2;
	uint8_t button_advance : 1;

	int16_t distance; // in millimeters
	int16_t angle;    // in degrees; counterclockwise is positive; clockwise is negative

	// Battery information
	uint8_t charging_state;
	uint16_t voltage; // mV
	int16_t current; // mA
	int8_t temperature; // Celcius
	uint16_t charge;
	uint16_t capacity; // mA-hrs

	// Light sensor signal strengths
	uint16_t wall_signal;
	uint16_t cliff_left_signal;
	uint16_t cliff_frontleft_signal;
	uint16_t cliff_frontright_signal;
	uint16_t cliff_right_signal;

	// Cargo bay info
	uint8_t cargo_bay_io0 : 1;
	uint8_t cargo_bay_io1 : 1;
	uint8_t cargo_bay_io2 : 1;
	uint8_t cargo_bay_io3 : 1;
	uint8_t cargo_bay_baud : 1;
	uint16_t cargo_bay_voltage;

	uint8_t internal_charger_on : 1;
	uint8_t home_base_charger_on : 1;

	uint8_t oi_mode; // off, passive, safe, full

	uint8_t song_number;
	uint8_t song_playing;

	uint8_t number_packets;
	int16_t requested_velocity;
	int16_t requested_radius;
	int16_t requested_right_velocity;
	int16_t requested_left_velocity;
} oi_t;


///Allocate and clear all memory for OI Struct
oi_t * oi_alloc();

///Free memory from pointer to Open Interface Struct
void oi_free(oi_t *self);


///Initialize open interface
void oi_init(oi_t *self);

///Update sensor data
void oi_update(oi_t *self);

/// \brief Set the LEDS on the Create
/// \param play_led 0=off, 1=on
/// \param advance_led 0=off, 1=on
/// \param power_color (0-255), 0=green, 255=red
/// \param power_intensity (0-255) 0=off, 255=full intensity
void oi_setLeds(uint8_t play_led, uint8_t advance_led, uint8_t power_color, uint8_t power_intensity);

/// \brief Set direction and speed of the robot's wheels
/// \param linear velocity in mm/s values range from -500 -> 500 of right wheel
/// \param linear velocity in mm/s values range from -500 -> 500 of left wheel
void oi_setWheels(int16_t right_wheel, int16_t left_wheel);


/// \brief Load song sequence
/// \param An integer value from 0 - 15 that acts as a label for note sequence
/// \param An integer value from 1 - 16 indicating the number of notes in the sequence
/// \param A pointer to a sequence of notes stored as integer values
/// \param A pointer to a sequence of durations that correspond to the notes
void oi_loadSong(int song_index, int num_notes, unsigned char  *notes, unsigned char  *duration);

/// \brief Play song
/// \param An integer value from 0 - 15 that is a previously establish song index
void oi_play_song(int index);

/// Calls in built in demo to send the iRobot to an open home base
/// This will cause the iRobot to enter the Passive state
void go_charge(void);

///	\brief Initialize UART3 for OI Communication and Debugging
///	internal function
void oi_uartInit(void);

///Set baud to 115200
void oi_uartFastMode(void);

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
