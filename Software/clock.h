#ifndef CLCK_H
#define CLCK_H

typedef struct
{
	uint8_t s;
	uint8_t m;
	uint8_t h;
	uint8_t dow;
	uint8_t d;
	uint8_t mon;
	uint8_t y;
	uint8_t mez;
	uint8_t snc;
}
zeit;

extern uint8_t time[60];
extern uint8_t time_n[60];
extern uint8_t dcf_i;
extern uint8_t dcf_synchron;
extern zeit uhrzeit;

void clock_init(void);
void s8_zeit(zeit uhrzeit, char* string);
void s20_zeit(zeit uhrzeit,char* string);
void clock_refresh(void);
uint8_t	check_parity(uint8_t value);
void clock_tick(void);
#endif
