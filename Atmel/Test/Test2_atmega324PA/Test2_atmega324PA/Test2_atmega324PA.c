/*
* Test3.c
*
* Test pentru transmitere/receptie prin bluetooth si pwm in functie de pachetul primit
*
* Created: 3/19/2015 8:35:17 PM
*  Author: chisa_000
*/


//#define RX_BUFFER_SIZE 3//128
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <ctype.h>
#include <io_definitions.h>
#include <PWM_lib/PWM_lib.h>
#include <Communication/communication.h>
#include <Sensors/sensors.h>
#include <Time/time.h>

/*
#define changePwm0A(duty) OCR0A = duty * (255/9)
#define changePwm0B(duty) OCR0B = duty * (255/9)
*/

/*
typedef struct{
	char data;
	char pack_no;
} info;

*/

//void initUSART1(int baud);
//void startPwm();
//void stopPwm();
//char getLastChar();

//volatile char rxBuffer[RX_BUFFER_SIZE];
/*volatile info rxBuffer[RX_BUFFER_SIZE];
volatile uint8_t rxReadPos=0;
volatile uint8_t rxWritePos=0;
*/

void executeCommands();
void initAll();

volatile int read=0;

int enableRight = 1;
int enableLeft = 1;
int turning=0;



void initAll(){
	initUSART1(9600);
	initTimer0();
	initTimer1();
	initTimer2();
	initSensors();
}


int main(void)
{
	
	initAll();
	/*
	initUSART1(9600);
	initTimer0();
	initTimer2();
	initSensors();
	*/

	
	LED_CMD_DDR |= (1<<LED_CMD_PINx);
	
	sei();

	breakAll();

	
	while(1)
	{
		executeCommands();		
	}
	
}


void executeCommands(){
		//startUltrasonicMeasure();
	int8_t s;
	command lastcmd = getLastCommand(&s);
	if(s==1){
		//CHECK CRC
		
		if(lastcmd.time){
			
			uint16_t arg1[] = {lastcmd.forward, lastcmd.forward};
			uint16_t arg2[2];
			arg2[0]= 0;
			arg2[1]=0;
			
			if(lastcmd.right>0){
				turnRight_90degrees();
			}else{
				//executeCommandForTime(&move, &move, 2, arg1, 2, arg2, lastcmd.time);
				changePwm_MotorLB(lastcmd.forward);
				changePwm_MotorRB(lastcmd.forward);
				changePwm_MotorLF(lastcmd.forward);
				changePwm_MotorRF(lastcmd.forward);
			}
			
			/*sendResponse(lastcmd.header);
			sendResponse(lastcmd.right);
			sendResponse(lastcmd.forward);
			sendResponse(lastcmd.time >> 8);
			sendResponse(lastcmd.time);*/
		}
		
		/*changePwm_MotorLB(lastcmd.forward);
		changePwm_MotorRB(lastcmd.forward);
		changePwm_MotorLF(lastcmd.forward);
		changePwm_MotorRF(lastcmd.forward);	
			*/
	}
	
	/*
	float US_distance =getAverageUltrasonicValue(5);// getLastUltrasonicValue();// getAverageUltrasonicValue(1);


	float IR = getDistance();
	
	if(IR<15.0 && (turning >> 1 ==0)){
		//turnRight_90degrees();
		turning|=1<<1;
		//breakAll();
		}else if((turning >>1 ==1) && IR>=15.0){
		turning&= ~(1<<1);
		
		stopTurning();
		changeMotorDirectionLB();
		changeMotorDirectionRB();
		changeMotorDirectionLF();
		changeMotorDirectionRF();
	
	}	
	

	if(US_distance<25.0 && ((turning & 1) == 0)){
		//turnRight_90degrees();
		turning|=1;
		//breakAll();
		}else if(((turning &1) ==1) && US_distance>=25.0){
		turning&=~1;
	
		stopTurning();
		changeMotorDirectionLB();
		changeMotorDirectionRB();
		changeMotorDirectionLF();
		changeMotorDirectionRF();
	
	}
	*/
	
}