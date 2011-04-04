#ifndef INTERRUPT_H
#define INTERRUPT_H

// Alle Variablen im bezug mit Interrupts tragen ein I im Namen 
extern volatile int8_t 	I_flag_1ms;
extern volatile int16_t I_DCF;
extern volatile int16_t I_DCF_P_DAUER;
extern volatile int8_t 	I_DCF_new;

void interrupt_init(void);
#endif
