/*
 * PWM_lib.c
 *
 * Created: 3/23/2015 8:41:35 PM
 *  Author: chisa_000
 */ 

#include <avr/io.h>

#include <io_definitions.h>
#include <PWM_lib/PWM_lib.h>
//#include "io_definitions.h"
//#include "PWM_lib.h"


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
	 

void changeMotorDirectionLF(){
	MOTOR_LF_DIR_PIN1 |= (1<<MOTOR_LF_DIR_PIN1x);
	MOTOR_LF_DIR_PIN2 |= (1<<MOTOR_LF_DIR_PIN2x);
}
	
void changeMotorDirectionLB(){
	MOTOR_LB_DIR_PIN1 |= (1<<MOTOR_LB_DIR_PIN1x);
	MOTOR_LB_DIR_PIN2 |= (1<<MOTOR_LB_DIR_PIN2x);
}
	  
void changeMotorDirectionRF(){
	MOTOR_RF_DIR_PIN1 |= (1<<MOTOR_RF_DIR_PIN1x);	
	MOTOR_RF_DIR_PIN2 |= (1<<MOTOR_RF_DIR_PIN2x);
}
	 
void changeMotorDirectionRB(){
	MOTOR_RB_DIR_PIN1 |= (1<<MOTOR_RB_DIR_PIN1x);
	MOTOR_RB_DIR_PIN2 |= (1<<MOTOR_RB_DIR_PIN2x);
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
	MOTOR_LF_DIR_DDR1 |= (1<<MOTOR_LF_DIR_PIN1x);
	MOTOR_LF_DIR_DDR2 |= (1<<MOTOR_LF_DIR_PIN2x);
	MOTOR_LB_DIR_DDR1 |= (1<<MOTOR_LB_DIR_PIN1x);
	MOTOR_LB_DIR_DDR2 |= (1<<MOTOR_LB_DIR_PIN1x);
	
	//setez directia initiala
	MOTOR_LF_DIR_PORT1 |= (1<<MOTOR_LF_DIR_PIN1x);
	MOTOR_LF_DIR_PORT2 &= ~(1<<MOTOR_LF_DIR_PIN2x);
	MOTOR_LB_DIR_PORT1 |= (1<<MOTOR_LB_DIR_PIN1x);
	MOTOR_LB_DIR_PORT2 &= ~(1<<MOTOR_LB_DIR_PIN2x);
	
	
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
	MOTOR_RF_DIR_DDR1 |= (1<<MOTOR_RF_DIR_PIN1x);
	MOTOR_RF_DIR_DDR2 |= (1<<MOTOR_RF_DIR_PIN2x);
	MOTOR_RB_DIR_DDR1 |= (1<<MOTOR_RB_DIR_PIN1x);
	MOTOR_RB_DIR_DDR2 |= (1<<MOTOR_RB_DIR_PIN2x);
	
	//setez directia initiala
	MOTOR_RF_DIR_PORT1 |= (1<<MOTOR_RF_DIR_PIN1x);
	MOTOR_RF_DIR_PORT2 &= ~(1<<MOTOR_RF_DIR_PIN2x);
	MOTOR_RB_DIR_PORT1 |= (1<<MOTOR_RB_DIR_PIN1x);
	MOTOR_RB_DIR_PORT2 &= ~(1<<MOTOR_RB_DIR_PIN2x);	
	
	
	
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