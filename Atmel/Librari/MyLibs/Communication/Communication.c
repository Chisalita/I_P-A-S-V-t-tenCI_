/*
* Communication.c
*
* Created: 4/7/2015 7:20:36 PM
*  Author: chisa_000
*/

#include <Communication/communication.h>
#include <Time/time.h>
#include <Sensors/sensors.h>
#include <math.h>
#include <io_definitions.h>
#include <avr/interrupt.h>
#include <util/crc16.h>

#define RX_BUFFER_SIZE 128//128
#define TX_BUFFER_SIZE 128//128
#define RX_BUFFER_SIZE_MSK 127
#define TX_BUFFER_SIZE_MSK 127


#define COMMAND_SIZE 7

typedef struct{
	uint8_t data;
	int8_t pack_no; // as putea pune int16_t pt a putea folosi valoarea de  -1
} info;

volatile info rxBuffer[RX_BUFFER_SIZE];
volatile uint8_t rxReadPos=0;
volatile uint8_t rxWritePos=0;
// folosit pentru a tine cont daca a inceput un pachet sau nu
volatile uint8_t packStarted = 0;
volatile uint8_t lastPackNo = 0;
volatile uint8_t lastPackPosInBuff = 0;
// this only saves the last header if it is not Status update!
volatile uint8_t lastCommandHeader = 0;



volatile info txBuffer[TX_BUFFER_SIZE];
volatile uint8_t txReadPos=0;
volatile uint8_t txWritePos=0;


void initUSART1(int baud){
	
	//init buffer
	/*int16_t i;
	for(i=0; i<RX_BUFFER_SIZE; i++){
	rxBuffer[i].pack_no =-1;
	
	///TEST////
	txBuffer[i].pack_no =-1;
	txBuffer[i].data =0;
	}*/
	__clearBuffers();
	

	USART1_RX_DDR &= ~(1<<USART1_RX_PINx);
	USART1_TX_DDR |= 1<<USART1_TX_PINx;
	
	//setting the UMSEL bit to 0 for asynchronous mode
	UCSR1C &= ~((1 << UMSEL10) | (1 << UMSEL11));

	UCSR1A &= ~(1 << U2X1); //setting the U2X bit to 0 for normal speed asynchronous
	
	// Set baud rate
	int ubbr_ = lrint(( (F_CPU) / (16L * baud) ) - 1L);
	UBRR1H = (unsigned char)(ubbr_>>8);
	UBRR1L = (unsigned char)ubbr_;
	
	// Enable receiver and transmitter
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	
	//8 bit data
	UCSR1C |= (1<<UCSZ10) | (1<<UCSZ11);
	UCSR1B &= ~(1<<UCSZ12);
	
	//1 stop bit
	UCSR1C&=~(1<<USBS1);
	
	// Parity mode - none
	UCSR1C &= ~((1 << UPM10) | (1<<UPM11));
	
	//Enable Receive Interrupt
	UCSR1B |= (1<<RXCIE1);
	//Enable Transmit Interrupt
	UCSR1B |= (1<<TXCIE1);
	
}

int8_t checkPackIsFullLength(uint8_t lastPackPos){
	uint8_t pc_no;
	
	if(rxBuffer[lastPackPos & RX_BUFFER_SIZE_MSK].pack_no == -1){
		return 0;
		}else{
		pc_no = rxBuffer[lastPackPos & RX_BUFFER_SIZE_MSK].pack_no;
	}

	
	//check if there is a valid pack and that all data corresponds to the same pack
	int8_t i;
	for(i=0;i< COMMAND_SIZE; i++){
		if(rxBuffer[(lastPackPos+i) & RX_BUFFER_SIZE_MSK].pack_no == -1
		|| rxBuffer[(lastPackPos+i) & RX_BUFFER_SIZE_MSK].pack_no != pc_no){
			return 0;
		}
	}
	return 1;
}

command fetchCommand(uint8_t lastPackPos, uint8_t *CRC_correct){
	
	
	//Aici se poate sa mai fie greseli din cauza semnelor (signed, unsigned)
	command comm = {.header = 0, .right = 0, .forward = 0, .time = 0, .CRC = 0};
	comm.header = rxBuffer[lastPackPosInBuff & RX_BUFFER_SIZE_MSK].data;
	comm.right = (int8_t) rxBuffer[(lastPackPosInBuff+1) & RX_BUFFER_SIZE_MSK].data; //recover the sign
	comm.forward = (int8_t) rxBuffer[(lastPackPosInBuff+2) & RX_BUFFER_SIZE_MSK].data;
	comm.time = rxBuffer[(lastPackPosInBuff+3) & RX_BUFFER_SIZE_MSK].data << 8;
	comm.time |= rxBuffer[(lastPackPosInBuff+4) & RX_BUFFER_SIZE_MSK].data;
	comm.CRC = rxBuffer[(lastPackPosInBuff+5) & RX_BUFFER_SIZE_MSK].data << 8;
	comm.CRC |= rxBuffer[(lastPackPosInBuff+6) & RX_BUFFER_SIZE_MSK].data;


	//check integrity with CRC
	int8_t i;
	uint16_t crc = 0xFFFF;
	for(i=0; i<COMMAND_SIZE; i++){
		crc = _crc16_update(crc, rxBuffer[(lastPackPosInBuff+i) & RX_BUFFER_SIZE_MSK].data);
		rxBuffer[(lastPackPosInBuff+i) & RX_BUFFER_SIZE_MSK].pack_no = -1; //erase data from buffer
	}
	
	if (crc == 0){
		*CRC_correct = 1;
		}else{
		*CRC_correct = 0;
	}
	
	return comm;
}

void sendResponse(response r){
	
	int8_t header = r.header;
	uint16_t i = txWritePos;
	
	
	txBuffer[i&TX_BUFFER_SIZE_MSK].data = header;
	txBuffer[i&TX_BUFFER_SIZE_MSK].pack_no = header;
	i++;
	txBuffer[i&TX_BUFFER_SIZE_MSK].data = NO_OF_SENSORS; // this must be sent!!
	txBuffer[i&TX_BUFFER_SIZE_MSK].pack_no = header;
	i++;
	
	for(uint8_t j=0; j<NO_OF_SENSORS; j++){
		txBuffer[i&TX_BUFFER_SIZE_MSK].data = r.sensorInfo[j];
		txBuffer[i&TX_BUFFER_SIZE_MSK].pack_no = header;
		i++;
	}
	
	txBuffer[i&TX_BUFFER_SIZE_MSK].data = (r.time >> 8); // first 8 bits
	txBuffer[i&TX_BUFFER_SIZE_MSK].pack_no = header;
	i++;
	
	txBuffer[i&TX_BUFFER_SIZE_MSK].data = (r.time & 0xFF); // next 8 bits
	txBuffer[i&TX_BUFFER_SIZE_MSK].pack_no = header;
	i++;
	
	//calculate CRC
	uint16_t crc = 0xFFFF;
	for(uint8_t j=0; j<NO_OF_SENSORS+4; j++){ // 4 = header, no_of_sensors, time(2)
		crc = _crc16_update(crc, txBuffer[(txWritePos+j) & TX_BUFFER_SIZE_MSK].data);
	}
	
	//send CRC in reverse order!
	txBuffer[i&TX_BUFFER_SIZE_MSK].data = (crc & 0xFF); // last 8 bits
	txBuffer[i&TX_BUFFER_SIZE_MSK].pack_no = header;
	i++;
	
	txBuffer[i&TX_BUFFER_SIZE_MSK].data = (crc >> 8); // first 8 bits
	txBuffer[i&TX_BUFFER_SIZE_MSK].pack_no = header;
	i++;
	txWritePos = i & TX_BUFFER_SIZE_MSK;
	
	__tryToSend();
}

void sendByte(uint8_t r){
	
	//UDR1 = r;
	if (UCSR1A & (1<<UDRE1))
	{
		UDR1 = r;
	}
	else
	{
		txBuffer[txWritePos].data = r;
		txBuffer[txWritePos].pack_no = 1;
		txWritePos++;
		
		txWritePos &=TX_BUFFER_SIZE_MSK;
		/*
		if(txWritePos >= TX_BUFFER_SIZE){
		txWritePos=0;
		}
		*/
	}
	
}

void __clearBuffers(){
	for(int16_t i=0; i<RX_BUFFER_SIZE; i++){
		rxBuffer[i].pack_no =-1;
		txBuffer[i].pack_no =-1;
		txBuffer[i].data =0;
	}
	txWritePos = 0;
	txReadPos = 0;
	rxReadPos = 0;
	rxWritePos = 0;
	lastCommandHeader = 0;
	lastPackNo = 0;
	lastPackPosInBuff = 0;
	packStarted = 0;
}

//this is used to send data so that USART1_TX_vect can be triggered
void __tryToSend(){
	if (UCSR1A & (1<<UDRE1)) //if data register is empty send a byte
	{
		cli();
		if(txBuffer[txReadPos].pack_no != -1){
			UDR1 = txBuffer[txReadPos].data;
			txBuffer[txReadPos].pack_no = -1;
			txBuffer[txReadPos].data = 0;
			txReadPos++;
			txReadPos &= TX_BUFFER_SIZE_MSK;
			/*
			if(txReadPos >= TX_BUFFER_SIZE){
			txReadPos=0;
			}
			*/
		}
		sei();
	}
}

command getLastCommand(int8_t *success){
	
	
	command cmd = {.header = 0, .right = 0, .forward = 0, .time = 0, .CRC = 0};
	*success = 0;
	uint8_t crc;
	
	if(checkPackIsFullLength(lastPackPosInBuff)){
		// read data and make command out of it
		
		cmd = fetchCommand(lastPackPosInBuff, &crc);
		
		if (crc)
		{
			*success=1;
			}else{
			*success=2;
		}
		
		
		
		}else{ //poate ca deja a inceput un nou pack dar nu a fost terminat,
		// atunci il returnez pe cel de dinainte care este intreg, daca exista un asemenea pack
									
		if( (lastPackPosInBuff - COMMAND_SIZE < RX_BUFFER_SIZE)
		&& checkPackIsFullLength(lastPackPosInBuff - COMMAND_SIZE)){
			// read data and make command out of it
			
			cmd = fetchCommand(lastPackPosInBuff - COMMAND_SIZE, &crc);
			
			if (crc)
			{
				*success=1;
				}else{
				*success=2;//it retrived something but the CRC was not matching
			}
			
			
		}
	}
	
	//if(cmd.header != Header_StatusUpdate){
		lastCommandHeader = cmd.header;
	//}
	
	return cmd;
}


void sendStatusUpdate(){
	
	response status;
	status.header = lastCommandHeader;
	//set info to send
	status.sensorInfo[0] = getInfraredForwardDistance();
	status.sensorInfo[1] = getInfraredRightDistance();
	status.sensorInfo[2] = getInfraredBackDistance();
	status.sensorInfo[3] = getInfraredLeftDistance();
	//add speed/distance
	
	uint16_t US_distance = getAverageUltrasonicValue(3);
	status.sensorInfo[NO_OF_SENSORS-2] = US_distance >> 8;
	status.sensorInfo[NO_OF_SENSORS-1] = US_distance & 0xff;
	status.time = getTimeExecutedLastCmd();
	//LED_CMD_PIN |= (1<<LED_CMD_PINx);
	sendResponse(status);
		
}

/*
char getLastChar(){
char ret = '\0';


if(rxBuffer[rxReadPos].pack_no != -1){
// read the character from buffer
ret = rxBuffer[rxReadPos].data;
}



// clear the character that was read
rxBuffer[rxReadPos].data = '\0';
rxBuffer[rxReadPos].pack_no = -1;


//increment position from which to read, if it reached the end set the position to start
rxReadPos++;
if(rxReadPos >= RX_BUFFER_SIZE){
rxReadPos = 0;
}

return ret;
}
*/

ISR(USART1_RX_vect){

	uint8_t data = UDR1;
	
	if(!packStarted){ // it is the first data byte of the pack
		
		LED_CMD_PIN |= (1<<LED_CMD_PINx);
		
		rxBuffer[rxWritePos].pack_no = data; //numai ultimi 7 biti va trebui sa ii consider
		rxBuffer[rxWritePos].data = data;
		packStarted = 1;
		lastPackNo = data;
		lastPackPosInBuff = rxWritePos;
		
				
		}else{
	
		rxBuffer[rxWritePos].pack_no = lastPackNo;
		rxBuffer[rxWritePos].data = data;
		packStarted++ ;
		
		if(packStarted == COMMAND_SIZE){//fara -3
			packStarted = 0;
		}
	}
	
	//rxBuffer[rxWritePos].data = UDR1;
	//rxBuffer[rxWritePos].pack_no = 1;
	rxWritePos++;
	rxWritePos &= RX_BUFFER_SIZE_MSK;
	
	/*if(rxWritePos>=RX_BUFFER_SIZE){
	rxWritePos = 0;
	}*/
	
	//LED_CMD_PIN |= (1<<LED_CMD_PINx);
	
}

ISR(USART1_TX_vect){
	
	
	//UDR1='I';
	if(txBuffer[txReadPos].pack_no != -1){
		UDR1 =  txBuffer[txReadPos].data;
		txBuffer[txReadPos].data =0;
		txBuffer[txReadPos].pack_no =-1;
		txReadPos++;
		txReadPos &= TX_BUFFER_SIZE_MSK;
		/*
		if(txReadPos>=TX_BUFFER_SIZE){
		txReadPos =0;
		}
		*/
	}
	
}