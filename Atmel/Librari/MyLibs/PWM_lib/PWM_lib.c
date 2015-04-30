/*
 * PWM_lib.c
 *
 * Created: 3/23/2015 8:41:35 PM
 *  Author: chisa_000
 */ 

#include <avr/io.h>

#include <io_definitions.h>
#include <PWM_lib/PWM_lib.h>
#include <Time/time.h>
////
#include <Communication/communication.h> //TESTE

void changePwm_MotorLF(int duty){	
	OCR0A = ((duty/100.0) * 255);
}

void changePwm_MotorLB(int duty){
 OCR0B = ((duty/100.0) * 255);
}

void changePwm_MotorRF(int duty){
OCR2A = ((duty/100.0) * 255);
}

void changePwm_MotorRB(int duty){
OCR2B = ((duty/100.0) * 255);
}
	 
void driveRightForTime(uint8_t pwm, uint16_t time){
	
	
	//check if it is a negative number and if it is call the complementary function
	if(pwm & (1<<((sizeof(pwm)*8) - 1))){ 
		//it is a negative pwm
		int8_t pwm_signed = pwm;
		pwm_signed = -pwm_signed;
		
		driveLeftForTime(pwm_signed, time);
		
	}else{
	
	int16_t arg1[] = {0xB0B0, 0x5050};
	int16_t arg2[3];	
	
	arg2[0] = pwm;
	arg2[0] |= pwm << 8;	
	arg2[1] = pwm;
	arg2[1] |= pwm << 8;	

	arg2[2] = time;

	executeCommandForTime(&move, &move, 2, arg1,3, arg2, TIME_TO_TURN_MS);//turn for TIME_TO_TURN_MS ms and then move forward for time ms
	}
	
}

void driveForwardForTime(uint8_t pwm, uint16_t time){
	
	//check if it is a negative number and if it is call the complementary function
	if(pwm & (1<<((sizeof(pwm)*8) - 1))){
		//it is a negative pwm
		int8_t pwm_signed = pwm;
		pwm_signed = -pwm_signed;
		driveBackwardForTime(pwm_signed, time);
	} 
	else
	{
		int16_t arg1[3];
		int16_t arg2[]= {0, 0};
		
		arg1[0] = pwm;
		arg1[0] |= pwm << 8;
		arg1[1] = pwm;
		arg1[1] |= pwm << 8;

		executeCommandForTime(&move, &move, 2, arg1,2, arg2, time);
	}	
	
}

void driveBackwardForTime(uint8_t pwm, uint16_t time){
	
		//check if it is a negative number and if it is call the complementary function
		if(pwm & (1<<((sizeof(pwm)*8) - 1))){
			//it is a negative pwm
			int8_t pwm_signed = pwm;
			pwm_signed = -pwm_signed;
			driveForwardForTime(pwm_signed, time);
		}
		else
		{
			int16_t arg1[3];
			int16_t arg2[]= {0, 0};
			
			// the pwm is for backward but expressed like forward
			// it must be reversed
			
			int8_t pwm_signed = pwm;
			pwm_signed = -pwm_signed;
			//change back to unsigned so that the bit operations work properly
			pwm = pwm_signed;
			
			arg1[0] = pwm;
			arg1[0] |= pwm << 8;
			arg1[1] = pwm;
			arg1[1] |= pwm << 8;
			
			executeCommandForTime(&move, &move, 2, arg1,2, arg2, time);
		}
	
	
}



void driveLeftForTime(uint8_t pwm, uint16_t time){
	
	//check if it is a negative number and if it is call the complementary function
	if(pwm & (1<<((sizeof(pwm)*8) - 1))){
		//it is a negative pwm
		int8_t pwm_signed = pwm;
		pwm_signed = -pwm_signed;
		
		driveRightForTime(pwm_signed, time);
		
		}else{
		
		int16_t arg1[] = {0x5050, 0xB0B0};
		int16_t arg2[3];
		
		arg2[0] = pwm;
		arg2[0] |= pwm << 8;
		arg2[1] = pwm;
		arg2[1] |= pwm << 8;

		arg2[2] = time;
		
		executeCommandForTime(&move, &move, 2, arg1,3, arg2, TIME_TO_TURN_MS);//turn for TIME_TO_TURN_MS ms and then move forward for time ms
	}
	
}

void driveRight(uint8_t pwm){
	//check if it is a negative number and if it is call the complementary function
	if(pwm & (1<<((sizeof(pwm)*8) - 1))){
		//it is a negative pwm
		int8_t pwm_signed = pwm;
		pwm_signed = -pwm_signed;
			
		driveLeft(pwm_signed);
			
		}else{
		int16_t arg1[] = {0xB0B0, 0x5050};
		int16_t arg2[2];
	
					///////////////Asta doar pt ca merge cu spatele//////////////////////////////
					int8_t pwm_signed = pwm;
					pwm_signed = -pwm_signed;
					//change back to unsigned so that the bit operations work properly
					pwm = pwm_signed;
					/////////////////////////////////////////////////////////////////////////////
	
		arg2[0] = pwm;
		arg2[0] |= pwm << 8;
		arg2[1] = pwm;
		arg2[1] |= pwm << 8;
	
		executeCommandForTime(&move, &move, 2, arg1,2, arg2, TIME_TO_TURN_MS);//turn for TIME_TO_TURN_MS ms and then move forward indefinitely
		}
}

void driveLeft(uint8_t pwm){
	
	//check if it is a negative number and if it is call the complementary function
	if(pwm & (1<<((sizeof(pwm)*8) - 1))){
		//it is a negative pwm
		int8_t pwm_signed = pwm;
		pwm_signed = -pwm_signed;
		
		driveRight(pwm_signed);
		
		}else{
		
		int16_t arg1[] = {0x5050, 0xB0B0};
		int16_t arg2[2];
		
		arg2[0] = pwm;
		arg2[0] |= pwm << 8;
		arg2[1] = pwm;
		arg2[1] |= pwm << 8;
		executeCommandForTime(&move, &move, 2, arg1,2, arg2, TIME_TO_TURN_MS);//turn for TIME_TO_TURN_MS ms and then move forward indefinitly
	}
	
}

void turnRight_90degrees(){	
	
	int16_t arg1[] = {0xB0B0, 0x5050};
	int16_t arg2[] = {0, 0};
	executeCommandForTime(&move, &move, 2, arg1,2, arg2, TIME_TO_TURN_MS);
	
	
}

void turnLeft_90degrees(){	
	
	int16_t arg1[] = {0x5050, 0xB0B0};
	int16_t arg2[] = {0, 0};
	executeCommandForTime(&move, &move, 2, arg1,2, arg2, TIME_TO_TURN_MS);
	
	
}


void stopTurning(){
		//setez directia initiala
		MOTOR_LEFT_DIR_DDR2 |= (1<<MOTOR_LEFT_DIR_PIN2x);
		MOTOR_LEFT_DIR_PORT1 &= ~(1<<MOTOR_LEFT_DIR_PIN1x);		

		MOTOR_RIGHT_DIR_PORT2 |= (1<<MOTOR_RIGHT_DIR_PIN2x);
		MOTOR_RIGHT_DIR_PORT1 &= ~(1<<MOTOR_RIGHT_DIR_PIN1x);
	
	
}

void reverse(){
	changeMotorDirectionLeft();
	changeMotorDirectionRight();

}

void move(uint16_t argc, int16_t* argv){
	
	if(argc == 2){
	//setez directia initiala
	MOTOR_LEFT_DIR_PORT2 |= (1<<MOTOR_LEFT_DIR_PIN2x);
	MOTOR_LEFT_DIR_PORT1 &= ~(1<<MOTOR_LEFT_DIR_PIN1x);
	
	MOTOR_RIGHT_DIR_PORT2 |= (1<<MOTOR_RIGHT_DIR_PIN2x);
	MOTOR_RIGHT_DIR_PORT1 &= ~(1<<MOTOR_RIGHT_DIR_PIN1x);
	
	int8_t d;
	
	d = argv[0] >> 8;
	
	if (d < 0){
		d=-d;
		if ((argv[0] & 0xff) < 0) // if both are negative numbers
		{
			changeMotorDirectionLeft();
		}
		
	}
	changePwm_MotorLF(d);
	
	
	d = argv[0] & 0xff;
	
	if (d < 0){ // aici pot pune un si
		d=-d;
	}
	changePwm_MotorLB(d);

	d = argv[1] >> 8;
	if (d < 0){
		d=-d;
		if ((argv[1] & 0xff) < 0)// if both are negative numbers
		{
			changeMotorDirectionRight();
		}
	}
	changePwm_MotorRF(d);
	
	d = argv[1] & 0xff;
	
	if (d < 0){
		d=-d;
	}	
	changePwm_MotorRB(d);
	}else if (argc == 3){
		
	int16_t arg1[2];
	arg1[0] = argv[0];
	arg1[1] = argv[1];
	
	int16_t arg2[] = {0, 0};
	executeCommandForTime(&move, &move, 2, arg1,2, arg2,argv[2]);
	
	}
	
}


void turnLeft(){
	changeMotorDirectionLeft();
}

void turnRight(){
	changeMotorDirectionRight();
}


void changeMotorDirectionLeft(){
	MOTOR_LEFT_DIR_PIN1 |= (1<<MOTOR_LEFT_DIR_PIN1x);
	MOTOR_LEFT_DIR_PIN2 |= (1<<MOTOR_LEFT_DIR_PIN2x);
}
	

	  
void changeMotorDirectionRight(){
	MOTOR_RIGHT_DIR_PIN1 |= (1<<MOTOR_RIGHT_DIR_PIN1x);	
	MOTOR_RIGHT_DIR_PIN2 |= (1<<MOTOR_RIGHT_DIR_PIN2x);
}
	 


void breakAll(){
	changePwm_MotorLF(0);
	changePwm_MotorLB(0);
	changePwm_MotorRF(0);
	changePwm_MotorRB(0);
}

void breakLeft(){
	changePwm_MotorLF(0);
	changePwm_MotorLB(0);
}

void breakRight(){
	changePwm_MotorRF(0);
	changePwm_MotorRB(0);
}

void initTimer0(){
	
	//setez pini de directie
	MOTOR_LEFT_DIR_DDR1 |= (1<<MOTOR_LEFT_DIR_PIN1x);
	MOTOR_LEFT_DIR_DDR2 |= (1<<MOTOR_LEFT_DIR_PIN2x);

	//setez directia initiala
	MOTOR_LEFT_DIR_PORT2 |= (1<<MOTOR_LEFT_DIR_PIN2x);
	MOTOR_LEFT_DIR_PORT1 &= ~(1<<MOTOR_LEFT_DIR_PIN1x);

	
	//Pt portul OC0A
	// Setting the pwm pin to output!
	//pt OC0A
	PWM_MOTOR_LF_DDR |= (1<<PWM_MOTOR_LF_PINx);
	
	//pt OC0B
	PWM_MOTOR_LB_DDR |= (1<<PWM_MOTOR_LB_PINx);
	
	// Clear on compare, set OC0A to Bottom (non-inverting mode)
	TCCR0A |= (1<< COM0A1);
	TCCR0A &= ~(1<<COM0A0);
	
	//pt OC0B
	TCCR0A |= (1<< COM0B1);
	TCCR0A &= ~(1<<COM0B0);
	
	// Set as Fast PWM (TOP = 0xFF, Update of OCRx at BOTTOM, TOV flag set at MAX)
	TCCR0A |= (1<<WGM00) | (1<<WGM01);
	
	// setez factorul de umplere
	changePwm_MotorLF(0);
	changePwm_MotorLB(0);
	
	// reset counter
	TCNT0 = 0;
	//enable clk/64 (From prescaler)
	TCCR0B = (1<<CS00)|(1<<CS01);
	
}

void initTimer2(){
	
	//setez pini de directie
	MOTOR_RIGHT_DIR_DDR1 |= (1<<MOTOR_RIGHT_DIR_PIN1x);
	MOTOR_RIGHT_DIR_DDR2 |= (1<<MOTOR_RIGHT_DIR_PIN2x);
	
	//setez directia initiala
	MOTOR_RIGHT_DIR_PORT2 |= (1<<MOTOR_RIGHT_DIR_PIN2x);
	MOTOR_RIGHT_DIR_PORT1 &= ~(1<<MOTOR_RIGHT_DIR_PIN1x);

	
	//Pt portul OC2A
	// Setting the pwm pin to output!
	//pt OC2A
	PWM_MOTOR_RF_DDR |= (1<<PWM_MOTOR_RF_PINx);
	
	//pt OC2B
	PWM_MOTOR_RB_DDR |= (1<<PWM_MOTOR_RB_PINx);
	
	// Clear on compare, set OC2A to Bottom (non-inverting mode)
	TCCR2A |= (1<< COM2A1);
	TCCR2A &= ~(1<<COM2A0);
	
	//pt OC2B
	TCCR2A |= (1<< COM2B1);
	TCCR2A &= ~(1<<COM2B0);
	
	// Set as Fast PWM (TOP = 0xFF, Update of OCRx at BOTTOM, TOV flag set at MAX)
	TCCR2A |= (1<<WGM20) | (1<<WGM21);
	
	// setez factorul de umplere
	changePwm_MotorRF(0);
	changePwm_MotorRB(0);

	// reset counter
	TCNT2 = 0;
	//enable clk/64 (From prescaler)
	TCCR2B &= ~((1<<CS20)|(1<<CS21));
	TCCR2B |= (1<<CS22);
}