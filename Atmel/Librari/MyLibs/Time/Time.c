/*
 * Time.c
 *
 * Created: 4/14/2015 11:56:36 AM
 *  Author: chisa_000
 */ 

#include <avr/io.h>
#include <>

void initTimer1()
{
	//Normal port operation, OC1A/OC1B disconnected.
    TCCR1A &= ~((1<<COM1A1) | (1<<COM1A0) | (1<<COM1B1) | (1<<COM1A0))  	
	
	//Timer/Counter mode of operation: CTC, TOP = OCR1A, Update of OCR1x at Immediate TOVn flag set on MAX
	TCCR1A &= ~((1<<WGM10) | (1<<WGM11));
	TCCR1B |= (1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	
	// Clock Select: no prescaler
	TCCR1B &= ~((1<<CS12) | (1<<CS11));
	TCCR1B |1= (1<<CS10);
	
	// reset counter
	TCNT1 = 0;
	
	// disable compare for the moment
	OCR1A = 0;
	OCR1B = 0;
	
	//enable Output Compare A and B Match Interrupt Enable
	TIMSK1 |= (1<<OCIE1B) | (1<<OCIE1A);
	
	
    return 0;
}