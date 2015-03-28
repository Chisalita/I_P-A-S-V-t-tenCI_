/*
 * PWM_lib.h
 *
 * Created: 3/23/2015 9:13:48 PM
 *  Author: chisa_000
 */ 


#ifndef PWM_LIB_H_
#define PWM_LIB_H_

#define changePwm_MotorLF(duty) OCR0A = ((duty/100.) * 255)
#define changePwm_MotorLB(duty) OCR0B = ((duty/100.) * 255)
#define changePwm_MotorRF(duty) OCR2A = ((duty/100.) * 255)
#define changePwm_MotorRB(duty) OCR2B = ((duty/100.) * 255)



void initTimer0();
void initTimer2();
void breakAll();
void breakLeft();
void breakRight();
void changeMotorDirectionLF();
void changeMotorDirectionLB();
void changeMotorDirectionRF();
void changeMotorDirectionRB();


#endif /* PWM_LIB_H_ */