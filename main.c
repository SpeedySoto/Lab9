
/*	Partner(s) Name & E-mail: David Wilson
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

void set_PWM(double frequency) {
	static double current_frequency; 
	if (frequency != current_frequency) {
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

enum States{CState, DState, EState, FState, GState, AState, BState, C5State}state;


unsigned char buttonPress = 0x00;

const double notes[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
unsigned char i = 0x00;

void button_Tick(){
	buttonPress = ~PINA & 0x07;
	switch(state)
	{ 
		case CState:
		if(buttonPress == 1)
		state = EState;
		else
		state = CState;
		break;
		case DState:
		if(!(buttonPress == 1))
		state = CState;
		else
		state = DState;
		break;
		case EState:
		if(buttonPress == 1)
		state = EState;
		else
		state = FState;
		break;
		case FState:
		if(buttonPress == 2)
		{
			if(i < 7)
			i++;
			state = GState;
		}
		else if(buttonPress == 4)
		{
			if(i > 0)
			i--;
			state = BState;
		}
		else if(buttonPress == 1)
		state = DState;
		else
		state = FState;
		break;
		case GState:
		state = AState;
		break;
		case AState:
		if(buttonPress == 2)
		state = AState;
		else
		state = FState;
		break;
		case BState:
		state = C5State;
		break;
		case C5State:
		if(buttonPress == 4)
		state = C5State;
		else
		state = FState;
		break;
	}
	
	
	switch(state)
	{ 
		case CState:
		PWM_off();
		break;
		case DState:
		break;
		case EState:
		PWM_on();
		break;
		case FState:
		break;
		case GState:
		set_PWM(notes[i]);
		break;
		case AState:
		break;
		case BState:
		set_PWM(notes[i]);
		break;
		case C5State:
		break;

	}
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; 
	DDRB = 0xFF; PORTB = 0x00; 
	PWM_on();
	state = CState;
	while(1){
		button_Tick();
	}
}

