#ifndef DREHGEBER_H
#define DREHGEBER_H

void encode_init(void);
int8_t encode_read1( void );         // read single step encoders
int8_t encode_read2( void );         // read two step encoders
int8_t encode_read4( void );         // read four step encoders
void drehgeber_auswertung_1ms();
uint8_t encode_press(void);
#endif
