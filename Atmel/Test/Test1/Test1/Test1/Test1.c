/*
 * GccApplication4.c
 *
 * Created: 3/6/2015 3:06:03 PM
 *  Author: chisa_000
 */ 


#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000


int main(void)
{
	
	int baud = 9600;
	UCSR1C &= ~(1 << UMSEL10); //setting the UMSEL bit to 0 for asynchronous mode
	UCSR1C &= ~(1 << UMSEL11); //setting the UMSEL bit to 0 for asynchronous mode
	int ubbr_ = lrint(( (F_CPU) / (16L * baud) ) - 1L);
	//UBBR = ubbr_;
	
	
	//UCSR0A = (1 << U2X0); //setting the U2X bit to 1 for double speed asynchronous
	
	UCSR1A &= ~(1 << U2X1); //setting the U2X bit to 0 for normal speed asynchronous
	
	// Set baud rate
	UBRR1H = (unsigned char)(ubbr_>>8);
	UBRR1L = (unsigned char)ubbr_;
	// Enable receiver and transmitter
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	// Set frame format: 8data, 2stop bit
	/////////UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	UCSR1C |= (3<<UCSZ10);
	UCSR1C&=~(1<<USBS1);
	
	// Pairity mode - none
	
	UCSR1C &= ~(1 << UPM10);
	UCSR1C &= ~(1 << UPM11);
	
	
	
	
	DDRA= (1<<5);
	asm("NOP");
	PINA=(1<<5);
	asm("NOP");
	char data;
	
	while(1)
	{
		
		//Recive
		while(! (UCSR1A & (1<<RXC1)));
		data = UDR1;
		//data+='0';
		//Transmit
		while(!(UCSR1A &(1<<UDRE1)));
		UDR1 = data;
		
		PINA=(1<<5);
		asm("NOP");
		_delay_ms(5000);
		PINA&=~(0x10);
		asm("NOP");
		
		//TODO:: Please write your application code
	}
	
}