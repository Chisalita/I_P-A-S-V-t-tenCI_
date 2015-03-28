/*
* Test3.c
*
* Test pentru transmitere/receptie prin bluetooth si pwm in functie de pachetul primit
*
* Created: 3/19/2015 8:35:17 PM
*  Author: chisa_000
*/


#define RX_BUFFER_SIZE 3//128
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <ctype.h>
#include "PWM_lib.h"
#include "io_definitions.h"


/*
#define changePwm0A(duty) OCR0A = duty * (255/9)
#define changePwm0B(duty) OCR0B = duty * (255/9)
*/


typedef struct{
	char data;
	char pack_no;
} info;


void initUSART1(int baud);
void initTimer0();
void startPwm();
void stopPwm();
char getLastChar();

//volatile char rxBuffer[RX_BUFFER_SIZE];
volatile info rxBuffer[RX_BUFFER_SIZE];
volatile uint8_t rxReadPos=0;
volatile uint8_t rxWritePos=0;

volatile int read=0;

int enableRight = 1;
int enableLeft = 1;


int main(void)
{
	
	initUSART1(9600);
	initTimer0();
	initTimer2();

/*	DDRA= (1<<PORTA3);
	PORTA |= (1<<PORTA3);
	
	DDRA= (1<<PORTA0);
	PORTA |= (1<<PORTA0);
	
	DDRA= (1<<PORTA1);
	PORTA &= ~(1<<PORTA1);
*/
	LED_CMD_DDR &= ~(1<<LED_CMD_PINx);
	
	
	
	//char data;
	
	sei();

	
	// startPwm(52);
	
	while(1)
	{
		/*
		//Recive
		while(! (UCSR1A & (1<<RXC1)));
		data = UDR1;
		//data+='0';
		//Transmit
		while(!(UCSR1A &(1<<UDRE1)));
		UDR1 = data;
		
		PINA|=(1<<PORTA3);
		_delay_ms(100);
		PINA|=(1<<PORTA3);
		
		
		*/
		
		//consume the buffer
		volatile	  char c = getLastChar();
		if(c != '\0'){
			switch(c){
				case '0':
					breakAll();
					/*
					changePwm_MotorLF(0);
					changePwm_MotorLB(0);
					changePwm_MotorRF(0);
					changePwm_MotorRB(0);
					*/
					//changePwm0A(0);
					//changePwm0B(0);
					break;
				case 'D':
				case 'd':
					enableRight ^= 1;
					break;
				case 'S':
				case 's':
					enableLeft ^= 1;
					break;
				case 'b':
				case 'B'://change direction
					PINA |= (1<<PORTA0);
					PINA |= (1<<PORTA1);
					break;
				default:
					if(isdigit(c)){
					
						if(enableLeft){
							changePwm_MotorLF(c-'0');
							changePwm_MotorLB(c-'0');
							//changePwm0A(c-'0');
							}else{
								breakLeft();
							/*
							changePwm_MotorLF(0);
							changePwm_MotorLB(0);
							*/
							//changePwm0A(0);
						
						}
						if(enableRight){
							changePwm_MotorRF(c-'0');
							changePwm_MotorRB(c-'0');
							//changePwm0B(c-'0');
						}
						else{
							breakRight();
							/*
							changePwm_MotorRF(0);
							changePwm_MotorRB(0);
							*/
							//changePwm0B(0);
						
						}
					
					
						}else{
							breakAll();
							/*changePwm_MotorLF(0);
							changePwm_MotorLB(0);
							changePwm_MotorRF(0);
							changePwm_MotorRB(0);
							*/
						//changePwm0A(0);
						//changePwm0B(0);
					}
				
					break;
			}
		}
		
		
		//TODO:: Please write your application code
	}
	
}

void initUSART1(int baud){

	DDRD &= ~(1<<PORTD2);
	
	
	//setting the UMSEL bit to 0 for asynchronous mode
	UCSR1C &= ~((1 << UMSEL10) | (1 << UMSEL11));

	UCSR1A &= ~(1 << U2X1); //setting the U2X bit to 0 for normal speed asynchronous
	
	// Set baud rate
	int ubbr_ = lrint(( (F_CPU) / (16L * baud) ) - 1L);
	UBRR1H = (unsigned char)(ubbr_>>8);
	UBRR1L = (unsigned char)ubbr_;
	
	// Enable receiver and transmitter
	UCSR1B = (1<<RXEN1);//|(1<<TXEN1);
	
	//8 bit data
	UCSR1C |= (1<<UCSZ10) | (1<<UCSZ11);
	UCSR1B &= ~(1<<UCSZ12);
	
	//1 stop bit
	UCSR1C&=~(1<<USBS1);
	
	// Parity mode - none
	UCSR1C &= ~((1 << UPM10) | (1<<UPM11));
	
	//Enable Receive Interrupt
	UCSR1B |= (1<<RXCIE1);
	
	
}

/*
void initTimer0(){
	
	//Pt portul OC0A
	// Setting the pwm pin to output!
	//pt OC0A
	DDRB |= (1<<PORTB3);
	
	//pt OC0B
	DDRB |= (1<<PORTB4);
	
	// Clear on compare, set OC0A to Bottom (non-inverting mode)
	TCCR0A |= (1<< COM0A1);
	TCCR0A &= ~(1<<COM0A0);
	
	//pt OC0B
	TCCR0A |= (1<< COM0B1);
	TCCR0A &= ~(1<<COM0B0);
	
	// Set as Fast PWM (TOP = 0xFF, Update of OCRx at BOTTOM, TOV flag set at MAX)
	TCCR0A |= (1<<WGM00) | (1<<WGM01);
	
	// setez factorul de umplere
	changePwm0A(0);
	changePwm0B(0);
	
	// reset counter
	TCNT0 = 0;
	//enable clk/64 (From prescaler)
	TCCR0B = (1<<CS00)|(1<<CS01);
	
	//Enable Overflow interrupt
	//TIMSK0 |= (1<<TOIE0);
	
	
	
}





void stopPwm(){
	// Normal port operation, OC0B disconnected
	TCCR0A &= ~((1<<COM0A0)|(1<<COM0A1));
	// No clock source (Timer/Counter stopped)
	TCCR0B &= ~((1<<CS00)|(1<<CS01)|(1<<CS02));
	// Setez pinul pe 0 pt ca nu stiu cum s-a oprit pwm-ul
	PORTB &= ~(1<<PORTB3);
}
*/

char getLastChar(){
	char ret = '\0';
	
	
	if(rxBuffer[rxReadPos].pack_no != -1){
		// read the character from buffer
		ret = rxBuffer[rxReadPos].data;
	}
	

	
	// clear the character that was read
	rxBuffer[rxReadPos].data = '\0';
	
	//increment position from which to read, if it reached the end set the position to start
	rxReadPos++;
	if(rxReadPos >= RX_BUFFER_SIZE){
		rxReadPos = 0;
	}
	
	return ret;
}

ISR(USART1_RX_vect){
	
	rxBuffer[rxWritePos].data = UDR1;
	rxBuffer[rxWritePos].pack_no = 1;
	rxWritePos++;
	
	read =1;
	if(rxWritePos>=RX_BUFFER_SIZE){
		rxWritePos = 0;
		
	}
	
	LED_CMD_PIN |= (1<<LED_CMD_PINx);
	
}