/*
 * time.h
 *
 * Created: 4/14/2015 1:10:10 PM
 *  Author: chisa_000
 */ 


#ifndef TIME_H_
#define TIME_H_

#define MAX_ARGS 10

void initTimer1();
void executeCommandForTime(void (*startF) (uint16_t, int16_t*), void (*stopF) (uint16_t, int16_t*),
uint16_t argc_start, int16_t* argv_start, uint16_t argc_stop, int16_t* argv_stop, uint16_t time);


#endif /* TIME_H_ */