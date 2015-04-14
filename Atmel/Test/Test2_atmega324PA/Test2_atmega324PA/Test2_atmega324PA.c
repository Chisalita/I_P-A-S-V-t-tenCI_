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


volatile int read=0;

int enableRight = 1;
int enableLeft = 1;
int turning=0;


int main(void)
{
	
	initUSART1(9600);
	initTimer0();
	initTimer2();
	initSensors();

/*	DDRA= (1<<PORTA3);
	PORTA |= (1<<PORTA3);
	
	DDRA= (1<<PORTA0);
	PORTA |= (1<<PORTA0);
	
	DDRA= (1<<PORTA1);
	PORTA &= ~(1<<PORTA1);
*/
	
	LED_CMD_DDR |= (1<<LED_CMD_PINx);
	//startUltrasonicMeasure();
//	setUltrasonicManualMeasure(1);
	//char data;
	
	sei();

	breakAll();
			
	// startPwm(52);
	
	while(1)
	{
		
		executeCommands();
		
		/*
		//consume the buffer
		volatile	  char c = getLastChar();
		if(c != '\0'){
			switch(c){
				case '0':
					breakAll();

					break;
				case 'Z':
				case 'z':

					turnLeft();
					break;
				case 'x':
				case 'X':
				
					turnRight();
					break;
				case 'D':
				case 'd':
					enableRight ^= 1;
					if(!enableRight){
						//breakRight();
						changePwm_MotorRB(0);
					}
					break;
				case 'S':
				case 's':
					enableLeft ^= 1;
					if (!enableLeft)
					{
						//breakLeft();
						changePwm_MotorLF(0);
					}
					break;
				case 'b':
				case 'B'://change direction
					changeMotorDirectionLF();
					changeMotorDirectionRF();
					changeMotorDirectionRB();
					changeMotorDirectionLB();
					break;
				default:
					if(isdigit(c)){
					//if(!isdigit(c)){
						if(enableLeft){
							changePwm_MotorLF((c-'0')*10);
							changePwm_MotorLB((c-'0')*10);
							//changePwm_MotorLF(c*10);
							//changePwm_MotorLB(c*10);
							}else{
								breakLeft();
						
						}
						if(enableRight){
							//changePwm_MotorRF(c*10);
							//changePwm_MotorRB(c*10);
							changePwm_MotorRF((c-'0')*10);
							changePwm_MotorRB((c-'0')*10);
						}
						else{
							breakRight();
						
						}
					
					
						}else{
							breakAll();

					}
				
					break;
			}
		}
	

		float d = getDistance();
		asm("nop");
		//if(d<10.0){
		if(d<15.0 && (!turning)){
			turnLeft();
			turning=1;
			//breakAll();
		}else if(turning && d>=15.0){
			turning=0;
			stopTurning();
			changeMotorDirectionLB();
			changeMotorDirectionRB();
			changeMotorDirectionLF();
			changeMotorDirectionRF();
		}
		
		*/
		//TODO:: Please write your application code
	}
	
}

void executeCommands(){
		//startUltrasonicMeasure();
	int8_t s;
	command lastcmd = getLastCommand(&s);
	if(s==1){
		//CHECK CRC
		
		changePwm_MotorLB(lastcmd.forward);
		changePwm_MotorRB(lastcmd.forward);
		changePwm_MotorLF(lastcmd.forward);
		changePwm_MotorRF(lastcmd.forward);		
	}
	
	float US_distance =getAverageUltrasonicValue(5);// getLastUltrasonicValue();// getAverageUltrasonicValue(1);


	float IR = getDistance();
	
	if(IR<15.0 && (turning >> 1 ==0)){
		turnLeft();
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
		turnLeft();
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
	
}