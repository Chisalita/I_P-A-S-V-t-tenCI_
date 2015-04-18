/*
 * PWM_lib.h
 *
 * Created: 3/23/2015 9:13:48 PM
 *  Author: chisa_000
 */ 


#ifndef PWM_LIB_H_
#define PWM_LIB_H_

void changePwm_MotorLF(int duty);
void changePwm_MotorLB(int duty);
void changePwm_MotorRF(int duty);
void changePwm_MotorRB(int duty);
void turnLeft();
void turnRight();


void initTimer0();
void initTimer2();
void breakAll();
void breakLeft();
void breakRight();
void changeMotorDirectionLF();
void changeMotorDirectionLB();
void changeMotorDirectionRF();
void changeMotorDirectionRB();
void stopTurning();

void move(uint16_t argc, int16_t* argv);
void turnRight_90degrees();
void turnLeft_90degrees();

#endif /* PWM_LIB_H_ */