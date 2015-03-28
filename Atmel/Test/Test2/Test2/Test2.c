/*
 * Test2.c
 *
 * Created: 3/12/2015 5:38:00 PM
 *  Author: chisa_000
 */ 


#include <avr/io.h>

int main(void)
{
	
	
	volatile int i =7;
	int* p= 0x23;
	
    while(1)
    {
		i++;
		*p = i;
        //TODO:: Please write your application code 
    }
}