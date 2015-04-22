/*
 * sensors.h
 *
 * Created: 4/7/2015 7:59:15 PM
 *  Author: chisa_000
 */ 


#ifndef SENSORS_H_
#define SENSORS_H_

#include <stdint.h>

void initADC();
void initUSART0(int baud);
uint8_t getDistance();
void initSensors();
uint16_t getLastUltrasonicValue();
uint16_t getAverageUltrasonicValue(uint8_t no_of_values);
void startUltrasonicMeasure();
void setUltrasonicManualMeasure(uint8_t val);



#endif /* SENSORS_H_ */