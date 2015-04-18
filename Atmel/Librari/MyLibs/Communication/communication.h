/*
 * communication.h
 *
 * Created: 4/7/2015 7:23:35 PM
 *  Author: chisa_000
 */ 


#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_
#include <stdint.h>

#define NO_OF_SENSORS 6
#define Header_StatusUpdate 128
#define Header_SpeedUpdate	255

typedef struct{
	uint8_t header;
	int8_t right;
	int8_t forward;
	uint16_t time;
	uint16_t CRC; 
	} command;


typedef struct{
	uint8_t header;
	uint8_t sensorInfo[NO_OF_SENSORS];
	uint16_t time;
	//uint16_t CRC;	
	} response;

void initUSART1(int baud);
//char getLastChar();
command getLastCommand(int8_t *success);
int8_t checkPackIsFullLength(uint8_t lastPackPos);
command fetchCommand(uint8_t lastPackPos, uint8_t *CRC_correct);
void sendResponse(response r);


//////////////////////////////////////////////////////////////////////////TEST
void sendByte(uint8_t r);

//Private functions:
void __tryToSend();
void __clearBuffers();


#endif /* COMMUNICATION_H_ */