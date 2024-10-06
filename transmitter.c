/*
 * main.c
 * Host: Banner Cut
 * Created: 3/26/2024 12:27:22 PM
 *  Author: Jacob Gerow
 *  For use by RUMC
 */ 

// 1 MHz
#define F_CPU 125000L

#include <xc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

void scream(void);
void play_nice(void);
void led_on(void);
void led_off(void);
void delay_25sms(int num);

void led_ready_indicate(void)
{

	for(int i = 0; i < 3; i++)
	{
		
		led_on();
		delay_25sms(10);
		led_off();
		delay_25sms(10);

	}

}


void setup_clock(void)
{

	CLKPR = 0x80;
	CLKPR = 0b00000110;			//Divide Clock frequency by 64

}

void setup_10s_timer(void)
{

	TCCR1 = 0b10001111;
	TIMSK = 0b01000000;		// Compare with A
	OCR1A = 65;				// Set limit


}

void setup_sleep(void)
{

	PRR |= ((1 << PRTIM0) | (1 << PRUSI) | (1 << PRADC)); // Power unnecessary Equipment down
	sleep_enable();

}

ISR(TIM1_COMPA_vect, ISR_NOBLOCK)
{

	led_on();
	delay_25sms(10);	//Quarter second LED flash
	led_off();

}

ISR(PCINT0_vect)
{
	if( (PINB & (1 << PINB0)) != 0)
	{
		//Debounce
		_delay_ms(20);
		if( ( PINB & (1 << PINB0) ) != 0)
		{
			play_nice();	
		}
	}
	
	GIFR = (1 << 5);	//Clear interrupt in case of funny business
}

void delay_25sms(int num)
{
	for(int i = 0; i < num; i++)
	{
		
		_delay_ms(25);
		
	}
}


void led_on(void)
{

	
	PORTB |= (1 << PINB2);

}

void led_off(void)
{

	PORTB &= ~(1 << PINB2);

}

// 2.5 seconds
void scream(void)
{
	for(int i = 0; i < 10; i++)
	{
		led_on();
		PORTB |= (1 << PB1);
		delay_25sms(8);			//Delay 200ms
		led_off();
		PORTB &= ~(1 << PB1);
		delay_25sms(2);			//Delay 50ms
		
	}
	
}

void transmit_1(void)
{
	
	PORTB |= (1 << PB1);

}

void transmit_0(void)
{

	PORTB &= ~(1 << PB1);

}

void send_0(void)
{
	
	transmit_1();
	_delay_ms(10);

	transmit_0();
	_delay_ms(20);

}

void send_1(void)
{

	transmit_1();
	_delay_ms(20);

	transmit_0();
	_delay_ms(10);

}

void play_nice(void)
{

	led_on();

	transmit_0();
	_delay_ms(10);

	for(int i = 0; i < 60; i++)
	{
		
		send_0();
		send_1();

	}

	led_off();

}


int main(void)
{
	setup_clock();
	setup_10s_timer();
	setup_sleep();
	// Push button on Pin 0
	// Transmitter Data out on Pin 1
	// LED on Pin 2
	DDRB =	0b00000110;
	GIMSK = 0b00100000;				// Enable Pin Change Interrupts
	PCMSK = 0b00000001;				// Enable Interrupt on PB0
	
	PORTB &= ~(1 << PB1);				// Output zero on transmitter
	PORTB &= ~(1 << PB2);				// LED off
	
	led_ready_indicate();

	sei();

    while(1)
    {
        //TODO:: Please write your application code
		sleep_cpu();
		
    }
}