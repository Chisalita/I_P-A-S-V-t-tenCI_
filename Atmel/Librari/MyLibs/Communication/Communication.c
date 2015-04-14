/*
 * Communication.c
 *
 * Created: 4/7/2015 7:20:36 PM
 *  Author: chisa_000
 */ 

#include <Communication/communication.h>
#include <math.h>
#include <io_definitions.h>
#include <avr/interrupt.h>
#include <util/crc16.h>

#define RX_BUFFER_SIZE 128//128
#define TX_BUFFER_SIZE 128//128
#define COMMAND_SIZE 7

typedef struct{
	int8_t data;
	int8_t pack_no;
} info;

volatile info rxBuffer[RX_BUFFER_SIZE];
volatile uint8_t rxReadPos=0;
volatile uint8_t rxWritePos=0;
// folosit pentru a tine cont daca a inceput un pachet sau nu
volatile uint8_t packStarted = 0;
volatile uint8_t lastPackNo = 0;
volatile uint8_t lastPackPosInBuff = 0;



///////////////TEST////////////////
volatile info txBuffer[TX_BUFFER_SIZE];
volatile uint8_t txReadPos=0;
volatile uint8_t txWritePos=0;


void initUSART1(int baud){
	
	//init buffer
	int16_t i;
	for(i=0; i<RX_BUFFER_SIZE; i++){
		rxBuffer[i].pack_no =-1;
		
		///TEST////
		txBuffer[i].pack_no =-1;
		txBuffer[i].data =0;
	}
	

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
	
	if(rxBuffer[(lastPackPos)% RX_BUFFER_SIZE].pack_no == -1){
		return 0;
	}else{
		pc_no = rxBuffer[(lastPackPos)% RX_BUFFER_SIZE].pack_no;
	}

	
	//check if there is a valid pack and that all data corresponds to the same pack
	int8_t i;
	for(i=0;i< COMMAND_SIZE; i++){
		if(rxBuffer[(lastPackPos+i)% RX_BUFFER_SIZE].pack_no == -1 
		|| rxBuffer[(lastPackPos+i)% RX_BUFFER_SIZE].pack_no != pc_no){
			return 0;
		}
	}
	return 1;
}

command fetchCommand(uint8_t lastPackPos, uint8_t *CRC_correct){
	
	command comm = {.header = 0, .right = 0, .forward = 0, .time = 0, .CRC = 0};
	comm.header = rxBuffer[lastPackPosInBuff % RX_BUFFER_SIZE].data;
	comm.right = (int8_t) rxBuffer[(lastPackPosInBuff+1) % RX_BUFFER_SIZE].data; //recover the sign
	comm.forward = (int8_t) rxBuffer[(lastPackPosInBuff+2) % RX_BUFFER_SIZE].data;
	comm.time = rxBuffer[(lastPackPosInBuff+3) % RX_BUFFER_SIZE].data << 8;
	comm.time |= rxBuffer[(lastPackPosInBuff+4) % RX_BUFFER_SIZE].data;
	comm.CRC = rxBuffer[(lastPackPosInBuff+5) % RX_BUFFER_SIZE].data << 8;
	comm.CRC |= rxBuffer[(lastPackPosInBuff+6) % RX_BUFFER_SIZE].data;

	int8_t i;
	//uint16_t crc = comm.CRC;
	uint16_t crc = 0xFFFF;
	for(i=0; i<COMMAND_SIZE; i++){
		crc = _crc16_update(crc, rxBuffer[(lastPackPosInBuff+i) % RX_BUFFER_SIZE].data);
		rxBuffer[(lastPackPosInBuff+i) % RX_BUFFER_SIZE].pack_no = -1;
	}
	
	if (crc == 0){
		*CRC_correct = 1;
		}else{
		*CRC_correct = 0;
	}
	
	return comm;
}


void sendResponse(char r){
	
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
		if(txWritePos > TX_BUFFER_SIZE){
			txWritePos=0;
		}
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
				*success=2;
			}
			
		
		}
	}
	
	
	return cmd;
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
		rxBuffer[rxWritePos].pack_no = data; //numai primi 7 biti
		rxBuffer[rxWritePos].data = data;
		packStarted = 1;
		lastPackNo = data;
		lastPackPosInBuff = rxWritePos; 
	}else{
		
		rxBuffer[rxWritePos].pack_no = lastPackNo;
		rxBuffer[rxWritePos].data = data;
		packStarted++ ;
		
		if(packStarted == COMMAND_SIZE){
			packStarted = 0;
		}
	}
	
	//rxBuffer[rxWritePos].data = UDR1;
	//rxBuffer[rxWritePos].pack_no = 1;
	rxWritePos++;
	
	if(rxWritePos>=RX_BUFFER_SIZE){
			rxWritePos = 0;
	}
	
	//LED_CMD_PIN |= (1<<LED_CMD_PINx);
	
}

ISR(USART1_TX_vect){
	
	
	//UDR1='I';
	if(txBuffer[txReadPos].pack_no != -1){
	UDR1 =  txBuffer[txReadPos].data;
	txBuffer[txReadPos].data =0;
	txBuffer[txReadPos].pack_no =-1;
	txReadPos++;
		if(txReadPos>TX_BUFFER_SIZE){
			txReadPos =0;
		}
	}
	
}