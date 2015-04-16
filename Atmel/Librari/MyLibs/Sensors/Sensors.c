/*
 * Sensors.c
 *
 * Created: 4/7/2015 7:58:47 PM
 *  Author: chisa_000
 */ 

#include <Sensors/sensors.h>
#include <io_definitions.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <ctype.h>
///////
#include <Communication/communication.h> //TESTE
#include <stdlib.h> //TESTE

#define MAX 3
#define MAX_ULTRASONIC_VALUES 10
volatile unsigned int i =0;
volatile float dis[MAX];
volatile float distance=0.0;

//ultrasonic
volatile float UltraSonicValues[MAX_ULTRASONIC_VALUES];
volatile float lastUSonicValue=0.0;
volatile uint8_t ultrasonicDataPack[4];
volatile uint8_t US_data_part_no = 0;
volatile uint8_t US_pos_in_buffer = 0;
volatile uint8_t US_manual_start = 0;


float getDistance(){
	
	distance=0.0;
	int j;
	for (j=0;j<MAX;j++)
	{
		distance += dis[j];
	}
	
	return distance/MAX;
}

float getLastUltrasonicValue(){
	return UltraSonicValues[US_pos_in_buffer-1];//because it is always the next position
}

float getAverageUltrasonicValue(uint8_t no_of_values){
	if (no_of_values > MAX_ULTRASONIC_VALUES){
		no_of_values = MAX_ULTRASONIC_VALUES;
	}
	
	float averrage = 0.0;
	uint8_t i;
	uint8_t good_vals=0;
	
	for (i=0; i<no_of_values; i++)
	{
		float aux;
		if(US_pos_in_buffer-i >= 0){ // rap around
			aux = UltraSonicValues[US_pos_in_buffer-i];
		}else{
			aux = UltraSonicValues[MAX_ULTRASONIC_VALUES - i + US_pos_in_buffer];
		}
		
		averrage += aux;
		
		if(aux){ // only if the value is nonzero
			good_vals++;
		}
		
	}
	
	averrage /= (float) good_vals;
	
	return averrage;
}

void setUltrasonicManualMeasure(uint8_t val){
	if (val){
		US_manual_start = 1;
		USART0_TX_PORT &= ~(1<<USART0_TX_PINx);
	} 
	else{
		US_manual_start = 0;
		
		//set TX to 1 to make the sensor measure
		USART0_TX_PORT |= 1<<USART0_TX_PINx;
	
	}
	
}

void startUltrasonicMeasure(){
	US_manual_start = 1;
	
	//set TX to 1 to make the sensor measure
	USART0_TX_PORT |= 1<<USART0_TX_PINx;
	
	
}

void initSensors(){
	
	//set sensor pins as input
	IR_SENSOR_F_DDR &= ~(1<<IR_SENSOR_F_PINx);
	IR_SENSOR_R_DDR &= ~(1<<IR_SENSOR_R_PINx);
	IR_SENSOR_B_DDR &= ~(1<<IR_SENSOR_B_PINx);
	IR_SENSOR_L_DDR &= ~(1<<IR_SENSOR_L_PINx);
	
	initADC();
	initUSART0(9600);
	//setUltrasonicManualMeasure(1);
	
}

void initUSART0(int baud){
	
	//init values
	
	uint8_t i;
	for (i=0; i<MAX_ULTRASONIC_VALUES; i++)
	{
		UltraSonicValues[i] = 0.0;
	}
	
	for (i=0; i<4; i++)
	{
		ultrasonicDataPack[i] = 0;
	}


	//rx pin as input and tx as out
	USART0_RX_DDR &= ~(1<<USART0_RX_PINx);
	USART0_TX_DDR |= 1<<USART0_TX_PINx;
	
	//set TX to 1 to make the sensor  measure
	USART0_TX_PORT |= 1<<USART0_TX_PINx;
	//USART0_TX_PORT &= ~(1<<USART0_TX_PINx);
	
	
	//setting the UMSEL bit to 0 for asynchronous mode
	UCSR0C &= ~((1 << UMSEL00) | (1 << UMSEL01));

	UCSR0A &= ~(1 << U2X0); //setting the U2X bit to 0 for normal speed asynchronous
	
	// Set baud rate
	int ubbr_ = lrint(( (F_CPU) / (16L * baud) ) - 1L);
	UBRR0H = (unsigned char)(ubbr_>>8);
	UBRR0L = (unsigned char)ubbr_;
	
	
	// Enable receiver do not enable transmitter because it will be high
	// and manual control of ultrasonic measurement will not take effect
	UCSR0B = (1<<RXEN0);
	
	//8 bit data
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	UCSR0B &= ~(1<<UCSZ02);
	
	//1 stop bit
	UCSR0C&=~(1<<USBS0);
	
	// Parity mode - none
	UCSR0C &= ~((1 << UPM00) | (1<<UPM01));
	
	
	//Enable Receive Interrupt
	UCSR0B |= (1<<RXCIE0); 
	
		
}

void  initADC(void)
{

	//set Vref to Vcc
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	//select ADC0
	
	ADMUX &= ~( (1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3) | (1<<MUX4) );

	//enable the ADC
	ADCSRA |= (1<<ADEN);
	
	//enable the ADC interrupt
	ADCSRA |= (1<<ADIE);
	
	//set prescaler to 128
	ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
	
	//select free running mode
	//ADCSRB &= ~( (1<<ADTS0) | (1<<ADTS1) | (1<<ADTS2) );


	//disable digital pins on sensor pins
	DIDR0 |= (1<<ADC0D) | (1<<ADC1D) | (1<<ADC2D) | (1<<ADC3D) ;


	//start conversion
	ADCSRA |= (1<<ADSC);

}

ISR(ADC_vect){
	
	uint8_t theLowADC = ADCL;
	uint16_t theTenBitResult = ADCH<<8 | theLowADC;
	
	if (theTenBitResult < 16)  theTenBitResult = 16;
	//return 2076.0 / (value - 11.0);
	
	//distance = (float) theTenBitResult;
	
	/*
	
	distance = 2076.0 / (theTenBitResult - 11.0);
	
	//under lower limit
	if (distance < 4.0)
	{
		distance =4.0;
	}
	
	//above upper limit
	if (distance > 30.0)
	{
		
		distance = 30.0;
	}
	*/
	
		
		dis[i%MAX] = 2076.0 / (theTenBitResult - 11.0);
		
		//under lower limit
		if (dis[i%MAX] < 4.0)
		{
			dis[i%MAX] =4.0;
		}
		
		//above upper limit
		if (dis[i%MAX] > 30.0)
		{
			
			dis[i%MAX] = 30.0;
		}
	
	
	i++;
	ADCSRA |= 1<<ADSC;
}

ISR(USART0_RX_vect){


	uint8_t data;
	data = UDR0;
	
	if(data == 0x52){ //start of data
		ultrasonicDataPack[0] = 'R';
		US_data_part_no=1;
	}else{
					
		if(ultrasonicDataPack[0] == 'R' && US_data_part_no == 4 && data == 13){// the pack is all read
			
			float val = 100.0 * ultrasonicDataPack[1] + 10.0 * ultrasonicDataPack[2] + ultrasonicDataPack[3];
			//transform from inch to cm
			val *= 2.54;
			UltraSonicValues[US_pos_in_buffer] = val;
			lastUSonicValue = val;
			US_pos_in_buffer++;

			if (US_pos_in_buffer>=MAX_ULTRASONIC_VALUES){ //reset buff pos
				US_pos_in_buffer =0;
			}
			
			//clear 
				uint8_t i;
				for (i=0; i<4; i++)
				{
					ultrasonicDataPack[i] = 0;
				}
				US_data_part_no=0;
				
				if(US_manual_start){//stop measuring if it is on manual
					USART0_TX_PORT &= ~(1<<USART0_TX_PINx);
				}
		}
		
		if(US_data_part_no >= 4){// it should be all read but probably
			// there was an error in transimission (there is no carriage return ASCII 13)
			//clear
			uint8_t i;
			for (i=0; i<4; i++)
			{
				ultrasonicDataPack[i] = 0;
			}
			US_data_part_no=0;
			
		
		}
		
		if(ultrasonicDataPack[0] == 'R'){ // read the next part of the pack
			if (isdigit(data))
			{//check for error in transmission
				ultrasonicDataPack[US_data_part_no] = data-'0';
				US_data_part_no++;
				
			}
			
			
		}
				
	}

}
