/*
 * algoritm.h
 *
 * Created: 5/5/2015 10:39:55 PM
 *  Author: chisa_000
 */ 


#ifndef ALGORITM_H_
#define ALGORITM_H_

#include <Communication/communication.h>
#include <Time/time.h>
#include <PWM_lib/PWM_lib.h>

void interpretCommands();
void __executeRecievedCommand(command cmd);

#endif /* ALGORITM_H_ */