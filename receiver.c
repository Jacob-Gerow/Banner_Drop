/*
 * main.c
 *
 * Created: 3/17/2024 9:49:56 PM
 *  Author: Jacob Gerow
 */ 


// 4 MHz
#define F_CPU 4000000L

#include <xc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>




char device_on = 0;

void light_off()
{
	
	PORTC.OUT = PORTC.OUT & 0b11111101;	//PC1 off

}

void light_on()
{
	
	PORTC.OUT = PORTC.OUT | 0b00000010;	//PC1 on

}

void motors_on(void)
{
	
	PORTD.OUT = PORTD.OUT | 0b00000011;

}

void motors_off(void)
{

	PORTD.OUT = PORTD.OUT & 0b11111100;

}

void switch_mode(void)
{
	
	if(device_on)
		{
			light_off();
			motors_off();
			device_on = 0;
		}
		else
		{
			light_on();
			motors_on();
			device_on = 1;
		}

}

ISR(PORTC_PORT_vect)
{

	//If button interrupt	
	if(PORTC.INTFLAGS & PIN0_bm)
	{
		_delay_ms(20);

		//Valid key press
		if(PORTC.IN & PIN0_bm)
		{
			switch_mode();
		}


		PORTC.INTFLAGS = PIN0_bm;

	}


}

int main(void)
{
	PORTC.DIR = 0b00000010;	//PC1 output for LED
	PORTC.PIN0CTRL = 0b00000010;	//Interrupt on for PORTC_PIN0
	

	PORTD.DIR = 0b00000011; // PD0, PD1 outputs

	sei();
	light_off();
	motors_off();

    while(1)
    {
		PORTC.DIR = 0b00000010;
		
        //TODO:: Please write your application code 
    }
}

