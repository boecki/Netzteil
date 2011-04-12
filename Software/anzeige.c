#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "adc.h"
#include "lcd_k0066.h"
#include "clock.h"
#include "interrupt.h"
#include "main.h"
#include "config.h"
#include "drehgeber.h"
#include "anzeige.h"

 
const char msg_1[] PROGMEM = "Melde-System-Test";
const char msg_2[] PROGMEM = "Mehr Test";
const char msg_3[] PROGMEM = "kurz";
 
const char *msg_storage[] PROGMEM = {
	msg_1,
	msg_2,
	msg_3
};

uint8_t message_count_down = 0;
uint8_t menu_counter = 0;
uint8_t menu_new = 1;
uint8_t menu_select = 0;
uint8_t msg_aktiv = 0;
// Darf nicht so groﬂ sien wie drehgeber_1
int16_t drehgeber_1_alt= 2341;


#define BUFFER_SIZE 10
 
struct Buffer {
  uint8_t data[BUFFER_SIZE];
  uint8_t read; // zeigt auf das Feld mit dem ‰ltesten Inhalt
  uint8_t write; // zeigt immer auf leeres Feld
} buffer = {{}, 0, 0};
 
uint8_t BufferIn(uint8_t byte)
{
  //if (buffer.write >= BUFFER_SIZE)
  //  buffer.write = 0; // erhˆht sicherheit
 
  if (buffer.write + 1 == buffer.read || (buffer.read == 0 && (buffer.write + 1 == BUFFER_SIZE)))
    return 0; // voll
 
  buffer.data[buffer.write] = byte;
 
  buffer.write = buffer.write + 1;
  if (buffer.write >= BUFFER_SIZE)
    buffer.write = 0;
  return 1;
 
}
 
uint8_t BufferOut(uint8_t *pByte)
{
  if (buffer.read == buffer.write)
    return 0;
  *pByte = buffer.data[buffer.read];
 
  buffer.read = buffer.read + 1;
  if (buffer.read >= BUFFER_SIZE)
    buffer.read = 0;
  return 1;
}


void message(uint8_t code)
{
	BufferIn(code);

}
void anzeige_refresh_100ms(void)
{
	// Pr¸fen auf mˆgliche Sondermeldungen und dann diese anzeigen
	if(message_count_down >0)
	{
		//Meldung wird momentan ausgegeben
		message_count_down--;

	}
	if(msg_aktiv == 0)
	{
		uint8_t *buffer = 0;		
		if(BufferOut(buffer) >0)
		{
			lcd_gotoxy(0,0);
			for(uint8_t i = 0; i < 20; i++)
			{
				lcd_puts_P(" ");
			}

			char work[21];
			lcd_gotoxy(0,0);
			// Es liegt eine Meldung vor und die wird nun ausgegeben
			strcpy_P( work, (const char*)( pgm_read_word( &(msg_storage[*buffer]) ) ) );

			lcd_puts(work);
			message_count_down = 15;
			msg_aktiv = 1;
		}
	}
	if(message_count_down == 0 && msg_aktiv >0)
	{
				lcd_gotoxy(0,0);
			// Keine Meldung vorhanden
			// Falls hier eben eine War muss hier aufger‰umt werden.
			// Vielleicht kann man das auch so machen das hier nur wenn eine wirklich da WAR
			// das ganze weg kommt;

			msg_aktiv = 0;
			menu_new = 1;
	}
	switch (menu_counter)
	{
		case MAIN_MENU:
			// Men¸ zeichnenen
			if(menu_new)
			{
				lcd_clrscr();
				lcd_gotoxy(0,0);
				lcd_puts_P("Hauptmenu");
				lcd_gotoxy(1,1);
				lcd_puts_P("Netzteil1 Netzteil2");
				lcd_gotoxy(1,2);
				lcd_puts_P("Laden 1   Laden 2");
				lcd_gotoxy(1,3);
				lcd_puts_P("TWIN-VIEW Optionen");
			}
			// Auswahlzeiger platzieren
			if((drehgeber_1_alt != drehgeber_1) || menu_new)
			{
				menu_select = (6+drehgeber_1%6)%6;
				lcd_gotoxy((menu_select%2)*10,(menu_select)/2+1);
				lcd_puts_P(">");
				if(!menu_new)
				{
					lcd_gotoxy(((6+drehgeber_1_alt%6)%2)*10,((6+drehgeber_1_alt%6)%6)/2+1);
					lcd_puts_P(" ");
				}
				drehgeber_1_alt = drehgeber_1;
			}
			menu_new = 0;
			// Dr¸ckauswertung
			if(!DREH_1_T)
			{
				menu_new = 1;
				menu_counter = (menu_select+menu_counter+1)*10;
				drehgeber_1 =0;
			}
		break;
		case NT_1_MENU:
			if(menu_new)
			{
				lcd_clrscr();
				// Achtung ¸ = ı
				lcd_home();
				lcd_puts_P(" zurıck NT1");
				lcd_gotoxy(1,1);
				lcd_puts_P("Us:     V Ui:     V");
				lcd_gotoxy(1,2);
				lcd_puts_P("Im:     A Ii:     A");
				lcd_gotoxy(1,3);
				lcd_puts_P("Pm:     W Pi:     W");
			}
			// Auswahlzeiger platzieren
			if((drehgeber_1_alt != drehgeber_1) || menu_new)
			{
				menu_select= (4+drehgeber_1%4)%4;
				// Neuen Pfeil setzen
				lcd_gotoxy(0,menu_select);
				lcd_puts_P(">");

				// Alte Pfeile entfernen, auﬂer wir haben gerade erst neugezeichnet
				if(!menu_new)
				{
					lcd_gotoxy(0,(4+drehgeber_1_alt%4)%4);
					lcd_puts_P(" ");
				}

				drehgeber_1_alt = drehgeber_1;
			}
			menu_new = 0;
			break;
		case LD_1_MENU:
			lcd_clrscr();
			lcd_gotoxy(3,1);
			lcd_puts_P("Software fehlt");
			if(DREH_1_T)
			{
				menu_new = 1;
				menu_counter = MAIN_MENU;				
			}
			
		break;

		case NT_2_MENU:
		case LD_2_MENU:
			lcd_clrscr();
			lcd_gotoxy(3,1);
			lcd_puts_P("Hardware fehlt");
			if(DREH_1_T)
			{
				menu_new = 1;
				menu_counter = MAIN_MENU;				
			}
			break;
		case TWIN_MENU:
		case OPTION_MENU:
		default:
			lcd_clrscr();
			lcd_gotoxy(3,1);
			lcd_puts_P("Menu unbekannt");
			if(DREH_1_T)
			{
				menu_new = 1;
				menu_counter = MAIN_MENU;				
			}
		break;
	}
}
void anzeige_clock (char* buffer)
{
	if(msg_aktiv ==0 && uhrzeit.snc == 2)
	{
		lcd_gotoxy(12,0);
		lcd_puts(buffer);
	}
}

