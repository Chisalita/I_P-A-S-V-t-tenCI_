/*
 * communication.h
 *
 * Created: 4/7/2015 7:23:35 PM
 *  Author: chisa_000
 */ 


#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_
#include <stdint.h>

typedef struct{
	uint8_t header;
	int8_t right;
	int8_t forward;
	uint16_t time;
	uint16_t CRC; 
	} command;

/*
typedef struct{
	uint8_t header;
	uint8_t no_of_sensors;
	////sensor information
	uint8_t sensorInfo_p[no_of_sensors];
	///
	uint16_t time;
	uint16_t CRC;	
	} response;
*/

void initUSART1(int baud);
//char getLastChar();
command getLastCommand(int8_t *success);
int8_t checkPackIsFullLength(uint8_t lastPackPos);
command fetchCommand(uint8_t lastPackPos, uint8_t *CRC_correct);


//////////////////////////////////////////////////////////////////////////TEST
void sendResponse(uint8_t r);

#endif /* COMMUNICATION_H_ */