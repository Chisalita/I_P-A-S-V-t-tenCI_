/*
 * Algoritm.c
 *
 * Created: 5/5/2015 10:39:09 PM
 *  Author: chisa_000
 */ 

#include <Algoritm/algoritm.h>

/////
#include <io_definitions.h>
////

#include <avr/io.h>
#include <stdlib.h>


void interpretCommands()
{
	int8_t success = 0;

    command lastcmd = getLastCommand(&success);
	
	if(success == 1){
		
		switch(lastcmd.header){
			case Header_StatusUpdate:
			sendStatusUpdate();
				break;
			default:
			__executeRecievedCommand(lastcmd);	
				break;
		}
				
	}


}


void __executeRecievedCommand(command cmd){
		LED_CMD_PIN |= (1<<LED_CMD_PINx);	
	if(cmd.time>0){
		
		 volatile uint16_t forward_time, right_time;
		float f_t, r_t, aux;
		
		f_t  = forward_time;
		r_t = right_time;
		aux = f_t/(f_t+r_t);
		forward_time = (uint16_t) (aux * cmd.time);

		//forward_time = (abs(cmd.forward)/(abs(cmd.forward)+abs(cmd.right))) * cmd.time;
		
		
		right_time = (abs(cmd.right)/(abs(cmd.forward)+abs(cmd.right))) * cmd.time;
		
		driveForwardForTime(cmd.forward, 2000);	// 2000 pt ca sunt ceva probleme cu variabila forward_time
		//start the timer
		resetTime();
		setTimeLimit(2000);
		startTime();
		
	}
	
	
}