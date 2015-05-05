#ifndef _IODEFINITIONS_INCLUDE
#define _IODEFINITIONS_INCLUDE

//Define the CPU frequency if it is not already defined
#ifndef F_CPU
#define F_CPU 8000000
#endif

#ifndef PRESCALER_TIMER_0_1
#define PRESCALER_TIMER_0_1 1024 //64
#endif


#include <avr/io.h>


// USART pins, directions and ports
#define USART0_RX_PINx PIND0
#define USART0_RX_PIN PIND
#define USART0_RX_DDR DDRD
#define USART0_RX_PORT PORTD

#define USART0_TX_PINx PIND1
#define USART0_TX_PIN PIND
#define USART0_TX_DDR DDRD
#define USART0_TX_PORT PORTD

#define USART1_RX_PINx PIND2
#define USART1_RX_PIN PIND
#define USART1_RX_DDR DDRD
#define USART1_RX_PORT PORTD

#define USART1_TX_PINx PIND3
#define USART1_TX_PIN PIND
#define USART1_TX_DDR DDRD
#define USART1_TX_PORT PORTD

//LED pins, directions and ports
/*#define LED_CMD_DDR DDRB
#define LED_CMD_PIN PINB
#define LED_CMD_PINx PINB1
#define LED_CMD_PORT PORTB
*/
#define LED_CMD_DDR DDRA
#define LED_CMD_PIN PINA
#define LED_CMD_PINx PINA0
#define LED_CMD_PORT PORTA

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
//#define MOTOR_LF_DIR_PIN1x PIND0
//#define MOTOR_LF_DIR_PIN2x PIND1
#define MOTOR_LEFT_DIR_PIN1x PINC0
#define MOTOR_LEFT_DIR_PIN2x PINC1
#define MOTOR_RIGHT_DIR_PIN1x PINC6
#define MOTOR_RIGHT_DIR_PIN2x PINC7

//#define MOTOR_LF_DIR_PIN1 PIND
//#define MOTOR_LF_DIR_PIN2 PIND
#define MOTOR_LEFT_DIR_PIN1 PINC
#define MOTOR_LEFT_DIR_PIN2 PINC
#define MOTOR_RIGHT_DIR_PIN1 PINC
#define MOTOR_RIGHT_DIR_PIN2 PINC


//#define MOTOR_LF_DIR_DDR1 DDRD
//#define MOTOR_LF_DIR_DDR2 DDRD
#define MOTOR_LEFT_DIR_DDR1 DDRC
#define MOTOR_LEFT_DIR_DDR2 DDRC
#define MOTOR_RIGHT_DIR_DDR1 DDRC
#define MOTOR_RIGHT_DIR_DDR2 DDRC

//#define MOTOR_LF_DIR_PORT1 PORTD
//#define MOTOR_LF_DIR_PORT2 PORTD
#define MOTOR_LEFT_DIR_PORT1 PORTC
#define MOTOR_LEFT_DIR_PORT2 PORTC
#define MOTOR_RIGHT_DIR_PORT1 PORTC
#define MOTOR_RIGHT_DIR_PORT2 PORTC


//Sensor pins, directions and ports
#define IR_SENSOR_F_PINx PINA4
#define IR_SENSOR_F_PIN PINA
#define IR_SENSOR_F_PORT PORTA
#define IR_SENSOR_F_DDR DDRA

#define IR_SENSOR_R_PINx PINA5
#define IR_SENSOR_R_PIN PINA
#define IR_SENSOR_R_PORT PORTA
#define IR_SENSOR_R_DDR DDRA

#define IR_SENSOR_B_PINx PINA6
#define IR_SENSOR_B_PIN PINA
#define IR_SENSOR_B_PORT PORTA
#define IR_SENSOR_B_DDR DDRA

#define IR_SENSOR_L_PINx PINA7
#define IR_SENSOR_L_PIN PINA
#define IR_SENSOR_L_PORT PORTA
#define IR_SENSOR_L_DDR DDRA


//////////////////////////////////////////////////////////////////////////
//Macros

#define changeMotrDirection(motor_dir_pin1x, motor_dir_pin2x, motor_dir_pin1, motor_dir_pin2) motor_dir_pin1 |= (1<<motor_dir_pin1x); \
motor_dir_pin2 |= (1<<motor_dir_pin2x)

#endif // _IODEFINITIONS_INCLUDE
