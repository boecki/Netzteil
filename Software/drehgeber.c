#include <avr/io.h>
#include <avr/interrupt.h>
#include "config.h"
#include "drehgeber.h"
int8_t enc_delta;          // -128 ... 127
static int8_t last;
static int8_t pressed;

void encode_init( void )
{
	PORTB |= (1<<PINB1) | (1<<PINB2) | (1<<PINB3);
  int8_t new;
 
  new = 0;
  if( DREH_1_A )
    new = 3;
  if( DREH_1_B )
    new ^= 1;                   // convert gray to binary
  last = new;                   // power on state
}
int8_t encode_read1( void )         // read single step encoders
{
  int8_t val;
 
  cli();
  val = enc_delta;
  enc_delta = 0;
  sei();
  return val;                   // counts since last call
}

int8_t encode_read2( void )         // read two step encoders
{
  int8_t val;
 
  cli();
  val = enc_delta;
  enc_delta = val & 1;
  sei();
  return val >> 1;
}
 
 
int8_t encode_read4( void )         // read four step encoders
{
  int8_t val;
 
  cli();
  val = enc_delta;
  enc_delta = val & 3;
  sei();
  return val >> 2;
}
uint8_t encode_press(void)
{
	if(pressed)
	{
		pressed = 0;
		return 1;	
	}
	return 0;
}

void drehgeber_auswertung_1ms()
{
  int8_t new, diff;
 
  new = 0;
  if( DREH_1_A )
    new = 3;
  if( DREH_1_B )
    new ^= 1;                   // convert gray to binary
  diff = last - new;                // difference last - new
  if( diff & 1 ){               // bit 0 = value (1)
    last = new;                 // store new as next last
    enc_delta += (diff & 2) - 1;        // bit 1 = direction (+/-)
	if(DREH_1_T)
	{
		pressed = 1;
	}
 }
}
