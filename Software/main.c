#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "adc.h"
#include "lcd_k0066.h"
#include "clock.h"
#include "interrupt.h"
#include "main.h"
#include "config.h"
#include "drehgeber.h"
#include "anzeige.h"





int8_t flag_10ms = 10;
int8_t flag_100ms = 10;
int8_t flag_1s = 10;
int16_t drehgeber_1 = 0;
//********************************************************************************
 
int main()
{
	uhrzeit.snc = 0;
	// Improvisierte Sollwertvorgabe auf 9V
	uint16_t soll =900;
//********************************************************************************

	// Initalisierung des Displays
	lcd_init(LCD_DISP_ON);
	//Initalisierung des ADC-Wandlers
	ADC_Init();
	// Interrupts initalisieren
	interrupt_init();
	// DCF-Uhr
	clock_init();
	// Drehgeber
	encode_init();

	


//********************************************************************************
	// Ausgänge für Strom und Spannungsreferenz konfigurieren;
	DDRD |= (1 << DD5) | (1 << DD6);
	//	Timer 1 A macht PWM mit 10bit auflösung auf Maximaler Frequenz
	//	PWM für die Spannungssollwert 
	TCCR1A |= (1<< COM1A1)| (1<<WGM11) | (1<<WGM10);
	TCCR1B |= (1<< WGM12) | (1<<CS10);

	//	Timer 2 B macht PWM mit 10bit auflösung auf Maximaler Frequenz
	//	PWM für die Stromsollwert
	TCCR2A |= (1<< COM2B1)| (1<<WGM21) | (1<<WGM20);
	TCCR2B |= (1<<CS20);

//********************************************************************************

	
	// Spannungs-PWM auf umgerechneten Sollwert setzen
	PWM_U_1 = ((soll*32)/47);
	// Ausgabe des aktuellen Wertes
	// lcd_int(ADC_Read(U_1_IST)*47/32);
	// Einfach einen Wert für die Strombegrenzung aussuchen
	PWM_I_1 = 0x88;

//********************************************************************************
// Initalisierung abgeschlossen, nun gehts in die Endlosschleife
	// Ab hier beginnt die Interupt behaftete Zone
	//lcd_puts_P("################################################################################");
  	sei();
	// initaliseren;
	message(0);
	message(0);



	while(1)
	{
		if(I_flag_1ms >0)
		{
			I_flag_1ms =0;
			flag_10ms--;

			// 1ms Taktrate ******************************************************
			clock_refresh();

			drehgeber_1 += encode_read4();
			 drehgeber_auswertung_1ms();
			
			// Provisorische Spannungsreglung
			if(ADC_Read(U_1_IST) <(soll*32)/47) OCR1A++;
			if(ADC_Read(U_1_IST) >(soll*32)/47) OCR1A--;
			// *******************************************************************
		}
		if(flag_10ms <0)
		{
			flag_10ms =9;
			flag_100ms--;

			// 10ms Taktrate *****************************************************


			// *******************************************************************

		}
		if(flag_100ms <0)
		{
			flag_100ms =9;
			flag_1s--;
			// 100ms Taktrate ****************************************************
			
			char buffer[20] = "\0";
		//	s20_zeit(uhrzeit, buffer);
		//	lcd_gotoxy(0,1);
			lcd_puts(buffer);
			char buffer_2[8] = "\0";
			s8_zeit(uhrzeit, buffer_2);
			anzeige_clock(buffer_2);
			anzeige_refresh_100ms();
							
			// *******************************************************************
			
		}
		if(flag_1s <0)
		{
			flag_1s =9;
			if(uhrzeit.s %2 == 1)
			{
				// Meldesystem - Stresstest
				//message(1);
			}
			// 1s Taktrate *******************************************************
			clock_tick();
			// *******************************************************************
		}
	}
	return 0;
}

// Alter Code
/*	{

		lcd_gotoxy(0,2);
		lcd_puts("     ");
		lcd_gotoxy(0,2);
		lcd_int(OCR1A);
		lcd_gotoxy(0,3);
		lcd_puts("     ");
		lcd_gotoxy(0,3);
		lcd_int(ADC_Read(0));
		
		lcd_gotoxy(6,1);
		lcd_puts("       ");
		lcd_gotoxy(6,1);
		lcd_int(ADC_Read(0)*47/32);

*/

