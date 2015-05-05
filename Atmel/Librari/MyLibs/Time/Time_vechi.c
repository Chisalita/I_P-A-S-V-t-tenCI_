/*
 * Time.c
 *
 * Created: 4/14/2015 11:56:36 AM
 *  Author: chisa_000
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <io_definitions.h>
#include <Time/time.h>

////
#include <Communication/communication.h> //TEST


//the function called when the timer begins
//void (*startFunction) (uint16_t argc, uint16_t* argv) = 0;
//the function called when the timer ends
void (*stopFunction) (uint16_t argc, int16_t* argv) = 0;

//number of parameters for stopFunction
volatile uint16_t argc2 = 0;
//the parameters for stopFunction
// it must be allocated memory!!
volatile int16_t argv2[MAX_ARGS];

// the number of ticks for the time
volatile uint32_t ticks = 0;


void executeCommandForTime(void (*startF) (uint16_t, int16_t*), void (*stopF) (uint16_t, int16_t*), 
uint16_t argc_start, int16_t* argv_start, uint16_t argc_stop, int16_t* argv_stop, uint16_t time){
	
	//copy arguments timer
	cli();
	stopFunction = stopF;
	argc2 = argc_stop;
	for(uint16_t i =0; i< argc_stop; i++){ // it needs to be manually copied!!!
		argv2[i] = argv_stop[i];
	}
	
	//settings for timer
	ticks  = ( ((F_CPU / 1000L) / PRESCALER_TIMER_0_1)*time ) -1;
	
	if(ticks > 0xFFFF){  //set the ticks variable
		OCR1A  = 0xFFFF;
		ticks -= 0xFFFF;
		}else{
		OCR1A = ticks;
		ticks = 0;
	}
	
	//start the function
	startF(argc_start, argv_start);
	LED_CMD_PORT |= (1<<LED_CMD_PINx);
		
	//start timer
		
	// reset counter
	TCNT1 = 0;
		
	//Clock Select: clk/64 prescaler
	TCCR1B &= ~((1<<CS11)); //1024
	TCCR1B |= (1<<CS10) | (1<<CS12);
	sei();
}

void initTimer1()
{
	//Normal port operation, OC1A/OC1B disconnected.
    TCCR1A &= ~((1<<COM1A1) | (1<<COM1A0) | (1<<COM1B1) | (1<<COM1B0));	
	
	//Timer/Counter mode of operation: CTC, TOP = OCR1A, Update of OCR1x at Immediate TOVn flag set on MAX
	TCCR1A &= ~((1<<WGM10) | (1<<WGM11));
	TCCR1B |= (1<<WGM12);
	TCCR1B &= ~(1<<WGM13);

	
	// reset counter
	TCNT1 = 0;
	
	//OCR1A = ((F_CPU / 1000L) / PRESCALER_TIMER_0_1)*10L;
	OCR1A = 0;
	OCR1B = (((F_CPU / 1000L) / PRESCALER_TIMER_0_1)*3000L) -1; //1 s
	
	//enable Output Compare A/B Match Interrupt Enable
	TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B); 

		// asta cred ca imi da totul peste cap la executeCommandForTime ...
		TCCR1B &= ~((1<<CS11)); //1024 ///////////////TEST
		TCCR1B |= (1<<CS10) | (1<<CS12);//////////////TEST
	
}

ISR(TIMER1_COMPA_vect){		
	if(ticks){
		if(ticks > 0xFFFF){
			OCR1A  = 0xFFFF;
			ticks -= 0xFFFF;
		}else{
			OCR1A = ticks;
			ticks = 0;
		}
	}else{
		
		//stop timer, it MUST be called first!!!
		TCCR1B &= ~((1<<CS10) | (1<<CS11) | (1<<CS12)); 
		OCR1A = 0; 
		LED_CMD_PORT &= ~(1<<LED_CMD_PINx);			
		stopFunction(argc2,argv2);						
	}		
}

ISR(TIMER1_COMPB_vect){
	//LED_CMD_PIN |= 1<<LED_CMD_PINx;
	TCNT1 = 0; // asta imi da totul peste cap la executeCommandForTime ...
	LED_CMD_PIN |= (1<<LED_CMD_PINx);
}