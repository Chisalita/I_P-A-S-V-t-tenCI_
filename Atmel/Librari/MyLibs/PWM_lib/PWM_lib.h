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
void changeMotorDirectionLeft();
void changeMotorDirectionRight();
void reverse();
void stopTurning();

void move(uint16_t argc, int16_t* argv);
void turnRight_90degrees();
void turnLeft_90degrees();

void driveRightForTime(uint8_t pwm, uint16_t time);
void driveLeftForTime(uint8_t pwm, uint16_t time);
void driveForwardForTime(uint8_t pwm, uint16_t time);
void driveBackwardForTime(uint8_t pwm, uint16_t time);

void driveRight(uint8_t pwm);
void driveLeft(uint8_t pwm);

void setTurningSpeed(uint16_t speed);
void setTurningTime(uint16_t time);
uint16_t getTurningTime();
uint16_t getTurningSpeed();

#endif /* PWM_LIB_H_ */