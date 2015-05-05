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
#include <Algoritm/algoritm.h>

//
#include <util/crc16.h> //TESTE

void executeCommands();
void initAll();

volatile int read=0;

int enableRight = 1;
int enableLeft = 1;
int turning=0;
int isAutonomous=0;

volatile uint8_t cm_IR=0;
volatile uint16_t mm_US =0;
volatile uint8_t car_speed = 0;


int bune =0; //teste


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
	LED_CMD_DDR |= (1<<LED_CMD_PINx);
	
	sei();

	breakAll();
	
	
	while(1)
	{
		executeScheduled();
		interpretCommands();
		//executeCommands();
	}
	
}


void executeCommands(){
	//startUltrasonicMeasure();
	int8_t s;
	command lastcmd = getLastCommand(&s);
	//sendStatusUpdate();
	//CHECK CRCs	
	if(s==1){
		//LED_CMD_PIN |= (1<<LED_CMD_PINx);
		//sendStatusUpdate();//// sar putea sa nu poata transmite atat de repede + intreruperile la 10 ms nu cred ca il lasa sa transmita bine...
						/*response resp;
						resp.header = lastcmd.header;
						resp.sensorInfo[0]=33;
						resp.sensorInfo[1]=33;
						resp.time=lastcmd.time;
						sendResponse(resp);		
	*/

		if(lastcmd.header == Header_StatusUpdate){ // is just status update
		
			sendStatusUpdate();

			
			}else{
			if(lastcmd.time){
				
				isAutonomous = 0;
				/*response resp;
				resp.header = lastcmd.header;
				resp.sensorInfo[0]=2;
				resp.sensorInfo[1]=3;
				resp.time=lastcmd.time;
				*/
				//sendResponse(resp);
				
				if(lastcmd.right!=0){
				//	driveRightForTime(lastcmd.right, lastcmd.time);//1
					}else{
				//	driveForwardForTime(lastcmd.forward, lastcmd.time);//1
				}
				}else{
				isAutonomous=1;
				car_speed = lastcmd.forward;
			
				//changePwm_MotorLB(car_speed);//1
				//changePwm_MotorRB(car_speed);
				//changePwm_MotorLF(car_speed);
				//changePwm_MotorRF(car_speed);
			
				
				
			}

		}

	}
	if(isAutonomous){
/*
		cm_IR = getInfraredRightDistance();
		mm_US = getAverageUltrasonicValue(3);
		
		if(cm_IR<20 && (turning >> 1 == 0)){
			//turnRight_90degrees();
			turnLeft();
			turning|=1<<1;
			//breakAll();
			}else if((turning >> 1 == 1) && cm_IR>=20){
			turning&= ~(1<<1);
			stopTurning();
			reverse();
			
			}else if(mm_US<250 && ((turning & 1) == 0)){
			//turnRight_90degrees();
			//	turnLeft();
			turning|=1;
			//breakAll();
			}else if(((turning &1) ==1) && mm_US>=250){
			turning&=~1;
			//		stopTurning();
			//		reverse();

		}
		*/

		/*
		//Ultima incercare
		cm_IR = getDistance();

		if(cm_IR<20 && (turning >> 1 == 0)){
		//turnRight_90degrees();
		mm_US = getAverageUltrasonicValue(3);
		if(mm_US<=200 && ((turning & 1) == 0)){
		//turnRight_90degrees();
		turnLeft_90degrees();
		turning|=1;
		//breakAll();
		}else if(((turning &1) == 0) && mm_US>200){
		turning|=1;
		turnRight_90degrees();
		}
		
		turning|=1<<1;
		//breakAll();
		}else if((turning >> 1 == 1) && cm_IR>=20){
		turning&= ~(1<<1);
		turning&=~1;
		stopTurning();
		changePwm_MotorLB(car_speed);
		changePwm_MotorRB(car_speed);
		changePwm_MotorLF(car_speed);
		changePwm_MotorRF(car_speed);
		reverse();
		
		}
		*/




	}
	
}