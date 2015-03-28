#ifndef _IODEFINITIONS_INCLUDE
#define _IODEFINITIONS_INCLUDE

#include <avr/io.h>

//LED pins, directions and potrs
#define LED_CMD_DDR DDRB
#define LED_CMD_PIN PINB
#define LED_CMD_PINx PINB1
#define LED_CMD_PORT PORTB

//Pwm pins, directions and ports(L = left, R = right, F = forward, B = back)
#define PWM_MOTOR_LF_PINx PINB3
#define PWM_MOTOR_LB_PINx PINB4
#define PWM_MOTOR_RF_PINx PIND7
#define PWM_MOTOR_RB_PINx PIND6

#define PWM_MOTOR_LF_PIN PINB
#define PWM_MOTOR_LB_PIN PINB
#define PWM_MOTOR_RF_PIN PIND
#define PWM_MOTOR_RB_PIN PIND

#define PWM_MOTOR_LF_DDR DDRB
#define PWM_MOTOR_LB_DDR DDRB
#define PWM_MOTOR_RF_DDR DDRD
#define PWM_MOTOR_RB_DDR DDRD

#define PWM_MOTOR_LF_PORT PORTB
#define PWM_MOTOR_LB_PORT PORTB
#define PWM_MOTOR_RF_PORT PORTD
#define PWM_MOTOR_RB_PORT PORTD

//direction select pins, directions and ports (L = left, R = right, F = forward, B = back)
#define MOTOR_LF_DIR_PIN1x PIND0
#define MOTOR_LF_DIR_PIN2x PIND1
#define MOTOR_LB_DIR_PIN1x PINB2
#define MOTOR_LB_DIR_PIN2x PINB5
#define MOTOR_RF_DIR_PIN1x PINC6
#define MOTOR_RF_DIR_PIN2x PINC7
#define MOTOR_RB_DIR_PIN1x PINC0
#define MOTOR_RB_DIR_PIN2x PINC1

#define MOTOR_LF_DIR_PIN1 PIND
#define MOTOR_LF_DIR_PIN2 PIND
#define MOTOR_LB_DIR_PIN1 PINB
#define MOTOR_LB_DIR_PIN2 PINB
#define MOTOR_RF_DIR_PIN1 PINC
#define MOTOR_RF_DIR_PIN2 PINC
#define MOTOR_RB_DIR_PIN1 PINC
#define MOTOR_RB_DIR_PIN2 PINC

#define MOTOR_LF_DIR_DDR1 DDRD
#define MOTOR_LF_DIR_DDR2 DDRD
#define MOTOR_LB_DIR_DDR1 DDRB
#define MOTOR_LB_DIR_DDR2 DDRB
#define MOTOR_RF_DIR_DDR1 DDRC
#define MOTOR_RF_DIR_DDR2 DDRC
#define MOTOR_RB_DIR_DDR1 DDRC
#define MOTOR_RB_DIR_DDR2 DDRC

#define MOTOR_LF_DIR_PORT1 PORTD
#define MOTOR_LF_DIR_PORT2 PORTD
#define MOTOR_LB_DIR_PORT1 PORTB
#define MOTOR_LB_DIR_PORT2 PORTB
#define MOTOR_RF_DIR_PORT1 PORTC
#define MOTOR_RF_DIR_PORT2 PORTC
#define MOTOR_RB_DIR_PORT1 PORTC
#define MOTOR_RB_DIR_PORT2 PORTC

//Sensor pins, directions and ports





//////////////////////////////////////////////////////////////////////////
//Macros

#define changeMotrDirection(motor_dir_pin1x, motor_dir_pin2x, motor_dir_pin1, motor_dir_pin2) motor_dir_pin1 |= (1<<motor_dir_pin1x); \
motor_dir_pin2 |= (1<<motor_dir_pin2x)

#endif // _IODEFINITIONS_INCLUDE
