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
	UCSR0C &= ~(1 << UMSEL00); //setting the UMSEL bit to 0 for asynchronous mode
	UCSR0C &= ~(1 << UMSEL01); //setting the UMSEL bit to 0 for asynchronous mode
	int ubbr_ = lrint(( (F_CPU) / (16L * baud) ) - 1L);
	//UBBR = ubbr_;
	
	
	//UCSR0A = (1 << U2X0); //setting the U2X bit to 1 for double speed asynchronous
	
	UCSR0A &= ~(1 << U2X0); //setting the U2X bit to 0 for normal speed asynchronous
	
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubbr_>>8);
	UBRR0L = (unsigned char)ubbr_;	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	/////////UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	UCSR0C |= (3<<UCSZ00);
	UCSR0C&=~(1<<USBS0);
	
	// Pairity mode - none
	
	UCSR0C &= ~(1 << UPM00); 
	UCSR0C &= ~(1 << UPM01);
	
	
	
	
	DDRB= (1<<3);
	asm("NOP");
	PINB=(1<<3);
	asm("NOP");
	char data;
	
	while(1)
    {
		
		/*PINB=(1<<3);
		asm("NOP");
		_delay_ms(5000);
		PINB=(0<<3);
		asm("NOP");
		_delay_ms(20);
	*/
		
		//Recive
		while(! (UCSR0A & (1<<RXC0)));
		data = UDR0;
		//data+='0';
		//Transmit
		while(!(UCSR0A &(1<<UDRE0)));
		UDR0 = data;
		
		PINB=(1<<3);
		asm("NOP");
		_delay_ms(5000);
		PINB=(0<<3);
		asm("NOP");
		
        //TODO:: Please write your application code 
    }
}