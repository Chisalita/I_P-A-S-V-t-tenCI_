#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

double dutyCycle=70;
volatile int ticks = 0;
int enabled =1;

void stoppwm();
void startpwm(double d_c);

int a;
double d;

int main(void)
{
	//////////////////////////////////////////////////////////////////////////
	d=60;
	//////////////////////////////////////////////////////////////////////////
	
	
	//asta trebuie sa fie inainte de a initializa PWM-ul
	DDRB |= (1<<PORTB3);
	
	// pt ain1 si ain2 de la driver
	DDRA |= (1<<PORTA1);
	DDRA |= (1<<PORTA0); 
	PORTA |= (1<<PORTA0);
	PORTA &= ~(1<<PORTA1);
	
	// Clear on compare, set OC0A to Bottom (non-inverting mode)
	TCCR0A |= (1<< COM0A1);

	a = 0xBF;
	TCCR0A &= a;
	 
	// Set as Fast PWM (TOP = 0xFF, Update of OCRx at BOTTOM, TOV flag set at MAX)
	TCCR0A |= 3;
	
	//Enable Overflow interrupt
	TIMSK0 |= (1<<TOIE0);
	
	//Set the value at which it interrupts 
	OCR0A = (dutyCycle/100)*255;
	
	sei();
	
	// No prescaler, it also starts the clock!	
	//TCCR0B = (1<<CS00)|(1<<CS02);
/*	startpwm(60);
	for (ticks=1;ticks<33500;ticks++)//30193
	{
		if(ticks+2<0){
			
			ticks++;
			break;
		}
		
		if (ticks==3345)
		{
			ticks+=2;
		}
	}
	ticks++;
//	_delay_ms(1000);
	ticks++; //2.96V
	stoppwm();
	ticks++;
	
	*/
	
    while(1)
    {	
		
		//_delay_ms(1000);
		
		if(d<90){
			d+=3;
		}else{
			d =0;
		}
	
		startpwm(d);
		_delay_ms(1000);
		stoppwm(); //d=72 pt 1000ms, 27 pt 100ms ,24 pt 10ms
		//dutyCycle = d;
		ticks++;
		_delay_ms(1000);
			
		//		startpwm(30);
		
		
        //TODO:: Please write your application code 
    }
}

void startpwm(double d_c){
	cli();
	
		TCCR0A |= (1<< COM0A1);

		a = 0xBF;
		TCCR0A &= a;
	
	dutyCycle = d_c;
	OCR0A = (d_c/100)*255;
	
	// reset counter
	TCNT0 = 0;
	//enable
	TCCR0B = (1<<CS00)|(1<<CS01);//|(1<<CS02);
	//_delay_ms(1000);
	//clear flags
	TIFR0 &= (1<<OCF0A)|(1<<TOV0);	
	sei();	
}

void stoppwm(){
	// trebuie sa incerc 
	
	
	TCCR0A &= ~((1<<COM0A0)|(1<<COM0A1));
	TCCR0B &= ~((1<<CS00)|(1<<CS01)|(1<<CS02));
	PORTB &= ~(1<<PORTB3);
	
	
	
	/*
	// ce ii jos ar trebui comentat si inlocuit cu ce ii sus....
	TCCR0B &= ~((1<<CS00)|(1<<CS01)|(1<<CS02));
	PORTB &= ~(1<<PORTB3);
	*/
}

ISR(TIMER0_OVF_vect){
	
	ticks++;
	
	
	//PORTA = !((PINA>>PORTA0)&1);
	//OCR0A = (dutyCycle/100)*255; 
}

