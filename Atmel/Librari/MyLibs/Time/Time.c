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
volatile uint32_t ticks = 99999;
volatile uint16_t lastCOMPARE = 0;


void executeCommandForTime(void (*startF) (uint16_t, int16_t*), void (*stopF) (uint16_t, int16_t*), 
uint16_t argc_start, int16_t* argv_start, uint16_t argc_stop, int16_t* argv_stop, uint16_t time){
	//init timer

	stopFunction = stopF;
	argc2 = argc_stop;
	for(uint16_t i =0; i< argc_stop; i++){ // it needs to be manually copied!!!
		argv2[i] = argv_stop[i];
	}
	
	////////////////////////////////////
	/*
	
	F_CPU tacturi .............. 1 s
	x tacturi ................... 10^-3 s
	
	x = F_CPU / 1000 (tacturi/ms)
	
	Compare Register = (milisecunde * x) / prescaler
	
	*/
	///////////////////////////////////
	
/*	volatile uint16_t x = F_CPU / 1000L;
	x = time * x * PRESCALER_TIMER_0_1;
*/

	//ticks = F_CPU / 1000L;
	//ticks = time * (F_CPU / 1000L) * PRESCALER_TIMER_0_1;
	//ticks = (time * (F_CPU / 1000L)) / PRESCALER_TIMER_0_1;
	//ticks++;
	//start timer
	
//	cli();
	
	/*if(ticks > 0xFFFF){
		OCR1A  = 0xFFFF;
		ticks -= 0xFFFF;
	}else{
		OCR1A = ticks;
		ticks = 0;
	}
	*/
	
	
		// Clock Select: clk/64 prescaler
//		TCCR1B &= ~((1<<CS12));
//		TCCR1B |= (1<<CS10) | (1<<CS11);

	//OCR1A = 0;
	//TCCR1B &= ~((1<<CS10) | (1<<CS11));

//	sei();		
	
	//call start function
	ticks = time;
	startF(argc_start, argv_start);
				LED_CMD_PORT |= (1<<LED_CMD_PINx);
}

void initTimer1()
{
	//Normal port operation, OC1A/OC1B disconnected.
    TCCR1A &= ~((1<<COM1A1) | (1<<COM1A0) | (1<<COM1B1) | (1<<COM1A0));	
	
	//Timer/Counter mode of operation: CTC, TOP = OCR1A, Update of OCR1x at Immediate TOVn flag set on MAX
	TCCR1A &= ~((1<<WGM10) | (1<<WGM11));
	TCCR1B |= (1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	
	// Clock Select: clk/64 prescaler
	TCCR1B &= ~((1<<CS12));
	TCCR1B |= (1<<CS10) | (1<<CS11);
	
	
	
	// reset counter
	TCNT1 = 0;
	
	// disable compare for the moment
	//OCR1A = 0;
	//OCR1B = 0;
	
	//set OCR1A for 1 ms interrupts
	OCR1A = ((F_CPU / 1000L) / PRESCALER_TIMER_0_1)*10L;
	
	//enable Output Compare A Match Interrupt Enable
	TIMSK1 |= (1<<OCIE1A);// (1<<OCIE1B) | (1<<OCIE1A);
	
}

ISR(TIMER1_COMPA_vect){
		
	//	LED_CMD_PIN |= (1<<LED_CMD_PINx);
		
		if(ticks && ticks != 99999){
			ticks--;
			//LED_CMD_PIN |= (1<<LED_CMD_PINx);
		}else if(ticks == 0){
			stopFunction(argc2,argv2);
			//LED_CMD_PIN |= (1<<LED_CMD_PINx);
			LED_CMD_PORT &= ~(1<<LED_CMD_PINx);
			ticks = 99999;
		}
		
/*		
	if(ticks>0){// > lastCOMPARE){
		//LED_CMD_PIN |= (1<<LED_CMD_PINx);
		if(ticks > 0xFFFF){
			sendResponse('R');
			lastCOMPARE = 0xFFFF;
			OCR1A = 0xFFFF;
			ticks -= lastCOMPARE; 
			TCCR1A = 0;
		}else{
			OCR1A = ticks;
			ticks = 0;
			TCCR1A = 0;
			sendResponse('Y');
		}
		
		//sendResponse('T');
		
	}else{

	//the time run out, stop the timer
	OCR1A = 0;
	TCCR1A = 0;
	TCCR1B &= ~((1<<CS10) | (1<<CS11));
	
	//call the stop function
	stopFunction(argc2, argv2);
	}

*/
}