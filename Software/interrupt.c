#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "interrupt.h"
#include "drehgeber.h"
#include "config.h"

volatile int8_t 	I_flag_1ms = 0;
volatile int16_t 	I_DCF = 0;
volatile int16_t 	I_DCF_P_DAUER = 0;
volatile int8_t 	I_DCF_new = 0;

void interrupt_init(void)
{
	//Timer0B sorgt-Interrupt alle 1ms	
	// CTC-Modus
	TCCR0A |= (1<<WGM01);
	// 64-Vorteiler
	TCCR0B |= (1<<CS01) | (1<<CS00);
	// Obere Grenze
	OCR0A = 186;
	// Interrupt bei Compare Match aktivieren
	TIMSK0 |= 1<<OCIE0B;


	// Pin-Change Interrupt auf PIN B0 einstellen
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT8);	
}
// 1ms
ISR(TIMER0_COMPB_vect)
{
	I_flag_1ms = 1;
	I_DCF++;
}
ISR(PCINT1_vect)
{
	I_DCF_P_DAUER = I_DCF;
	I_DCF_new =1;
	I_DCF = 0;
}
