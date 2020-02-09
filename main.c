
/*	Partner(s) Name & E-mail: David Wilson
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #1 
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

void set_PWM(double frequency)
 {
	static double current_frequency; 
	if (frequency != current_frequency)
	 {
		if (!frequency) { TCCR0B &= 0x08; } 
		else { TCCR0B |= 0x03; } 

		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		

		else if (frequency > 31250) { OCR0A = 0x0000; }

		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; 
		current_frequency = frequency; 
	}
}

void PWM_on()
 {
	TCCR0A = (1 << COM0A0 | 1 << WGM00);

	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);

	set_PWM(0);
}

void PWM_off() 
{
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}

enum States{Hold, CState, DState, EState}state;

unsigned char buttonPress = 0x00;

void button_Tick()
{
	buttonPress = ~PINA & 0x07;
	switch(state)
	{ 
		case Hold:
		if(buttonPress == 1)
		{
			state = CState;
		}
		else if(buttonPress == 2)
		{
			state = DState;
		}
		else if(buttonPress == 4)
		{
			state = EState;
		}
		else{
			state = Hold;
		}
		break;
		case CState:
		if(buttonPress == 1)
		{
			state = CState;
		}
		else{
			state = Hold;
		}
		break;
		case DState:
		if(buttonPress == 2)
		{
			state = DState;
		}
		else{
			state = Hold;
		}
		break;
		case EState:
		if(buttonPress == 4)
		{
			state = EState;
		}
		else{
			state = Hold;
		}
		break;
	}
	switch(state)
	{
		case Hold:
		set_PWM(0);
		break;
		case CState:
		set_PWM(261.63);
		break;
		case DState:
		set_PWM(293.66);
		break;
		case EState:
		set_PWM(329.63);
		break;
	}
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // A input initialized to 0xFF
	DDRB = 0xFF; PORTB = 0x00; // B output initialized to 0x00
	PWM_on();
	state = Hold;
	while(1){
		button_Tick();
	}
} 