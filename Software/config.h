#ifndef CONFIG_H
#define CONFIG_H




/* Fehlerlevel
*
*	NOTICE 			= Hinweis z.B. Irgendetwas wurde abgeschlossen (Uhren synchronisation)
*	WARNING 		= Warnung z.B. Zulässige Betriebsparameter werden leicht überschritten
*	ERROR			= Fehler  z.B. Zulässige Betriebsparameter werden stark überschritten
*	FATAL_ERROR 	= schwerer Fehler z.B. Leitungsbrüche Kurzschlüsse
*
*/

#define NOTICE 		0
#define WARNING		1
#define ERROR		2
#define FATAL_ERROR	3

// Eingänge & Ausgänge;
#define U_1_IST 0
#define I_1_IST 1

#define PWM_U_1 OCR1A
#define PWM_I_1 OCR2B

#define DREH_1_A	(PINB & 1<<PB1)
#define DREH_1_B	(PINB & 1<<PB2)
#define DREH_1_T	(PINB & 1<<PB3)

#define MAIN_MENU	0
#define NT_1_MENU	10
#define NT_2_MENU	20
#define LD_1_MENU	30
#define LD_2_MENU	40
#define TWIN_MENU 	50
#define OPTION_MENU	60


#endif
