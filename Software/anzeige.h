#ifndef ANZEIGE_H
#define ANZEIGE_H
extern uint8_t message_count_down;

void message(uint8_t code);
void anzeige_refresh_100ms(void);
void anzeige_clock(char* buffer);
#endif
