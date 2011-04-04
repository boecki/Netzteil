#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "lcd_k0066.h"
#include "interrupt.h"

#include "config.h"
#include "anzeige.h"
#include "main.h"


uint8_t time[60];
uint8_t time_n[60];
uint8_t dcf_i =0;
uint8_t dcf_synchron =0;
zeit uhrzeit;

void clock_init(void)
{
	uhrzeit.snc = 0;
	uhrzeit.d = 0;
	uhrzeit.mon = 0;
	uhrzeit.y = 11;

	uhrzeit.h = 0;
	uhrzeit.m = 0;
	uhrzeit.s = 0;
}

//String der mit Formatierung HH:MM:SS
//00:00:00
void s8_zeit(zeit uhrzeit, char* string)
{
	// Speichern für itoa
	char buffer[3];

	if(uhrzeit.h >9)
	{
		itoa(uhrzeit.h, buffer,10);
		strcat(string, buffer);
	}
	else
	{
		string[0] = ' ';
		itoa(uhrzeit.h, buffer,10);
		strcat(string, buffer);
	}
	string[2] = ':';
	if(uhrzeit.m >9)
	{
		itoa(uhrzeit.m, buffer,10);
		strcat(string, buffer);
	}
	else
	{
		string[3] = '0';
		itoa(uhrzeit.m, buffer,10);
		strcat(string, buffer);
	}
	string[5] = ':';
	if(uhrzeit.s >9)
	{
		itoa(uhrzeit.s, buffer,10);
		strcat(string, buffer);
	}
	else
	{
		string[6] = '0';
		itoa(uhrzeit.s, buffer,10);
		strcat(string, buffer);
	}
	string[8]='\0';
}
//String der mit Formatierung WT TT.MM.JJ HH:MM:SS
//Mo 00.00.00 00:00:00
void s20_zeit(zeit uhrzeit, char* string)
{
	// Speichern für itoa
	char buffer[3];

	// Wochentag
	switch(uhrzeit.dow)
	{
		case 1:
			strcat(string, "Mo");
		break;
		case 2:
			strcat(string, "Di");
		break;
		case 3:
			strcat(string, "Mi");
		break;
		case 4:
			strcat(string, "Do");
		break;
		case 5:
			strcat(string, "Fr");
		break;
		case 6:
			strcat(string, "Sa");
		break;
		case 7:
			strcat(string, "So");
		break;
		default:
			strcat(string, "ND");
		break;
	}
	string[2] = ' ';
	if(uhrzeit.d >9)
	{
		itoa(uhrzeit.d, buffer,10);
		strcat(string, buffer);
	}
	else
	{
		string[3] = ' ';
		itoa(uhrzeit.d, buffer,10);
		strcat(string, buffer);
	}
	string[5] = '.';
	if(uhrzeit.mon >9)
	{
		itoa(uhrzeit.mon, buffer,10);
		strcat(string, buffer);
	}
	else
	{
		string[6] = '0';
		itoa(uhrzeit.mon, buffer,10);
		strcat(string, buffer);
	}

	string[8] = '.';
	if(uhrzeit.y >9)
	{
		itoa(uhrzeit.y, buffer,10);
		strcat(string, buffer);
	}
	else
	{
		string[9] = '0';
		itoa(uhrzeit.y, buffer,10);
		strcat(string, buffer);
	}
	string[11] = ' ';
	if(uhrzeit.h >9)
	{
		itoa(uhrzeit.h, buffer,10);
		strcat(string, buffer);
	}
	else
	{
		string[12] = ' ';
		itoa(uhrzeit.h, buffer,10);
		strcat(string, buffer);
	}
	string[14] = ':';
	if(uhrzeit.m >9)
	{
		itoa(uhrzeit.m, buffer,10);
		strcat(string, buffer);
	}
	else
	{
		string[15] = '0';
		itoa(uhrzeit.m, buffer,10);
		strcat(string, buffer);
	}
	string[17] = ':';
	if(uhrzeit.s >9)
	{
		itoa(uhrzeit.s, buffer,10);
		strcat(string, buffer);
	}
	else
	{
		string[18] = '0';
		itoa(uhrzeit.s, buffer,10);
		strcat(string, buffer);
	}
	string[20]='\0';
	return;
}
void clock_refresh(void)
{
	if(I_DCF_new)
	{
		cli();
		int16_t DCF_dauer = I_DCF_P_DAUER;
		sei();

		if(DCF_dauer >50 && (DCF_dauer <150))
		{
			time_n[dcf_i] = 0;
			dcf_i++;
		}
		if(DCF_dauer >150 && (DCF_dauer <250))
		{
			time_n[dcf_i] = 1;
			dcf_i++;
		}
		if(DCF_dauer >1700)
		{
			dcf_i = 0;
			//TODO Paritätsbits prüfen und Fehlerabgleich mit alter Uhrzeit, fals vorhanden;
			if(uhrzeit.snc >= 1)
			{


				
				for(uint8_t i = 0; i <60; i++)
				{
					time[i] = time_n[i];
				}

				uhrzeit.m = time[21]+2*time[22]+4*time[23]+8*time[24]+ 10*(time[25]+2*time[26]+4*time[27]);
				uhrzeit.h = time[29]+2*time[30]+4*time[31]+8*time[32]+ 10*(time[33]+2*time[34]);
				uhrzeit.d = time[36]+2*time[37]+4*time[38]+8*time[39]+ 10*(time[40]+2*time[41]);
				uhrzeit.dow = time[42]+2*time[43]+4*time[44];
				uhrzeit.mon = time[45]+2*time[46]+4*time[47]+8*time[48]+ 10*(time[49]);
				uhrzeit.y = time[50]+2*time[51]+4*time[52]+8*time[53]+ 10*(time[54]+2*time[55]+4*time[56]+8*time[57]);
				uhrzeit.s = 0;
				flag_10ms = 9;
				flag_100ms = 9;
				flag_1s = 9;
				
				uhrzeit.snc = 2;
			}
			else
			{
				uhrzeit.snc = 1;
			}
		}
		if(DCF_dauer > 3000)
		{
			
		}
		if(dcf_i >=60)
		{
			dcf_i = 0;
		}
		if (uhrzeit.snc >= 2)
		{
			//uhrzeit.s = dcf_i -1;
		}
		I_DCF_new =0;
	}
}
uint8_t	check_parity(uint8_t value)
{
	uint8_t	even_parity = 0;

	while(value)
	{
		even_parity ^= (value & 0x01);
		value >>= 1; 
	};
	return even_parity;

}
void clock_tick(void)
{
	uhrzeit.s++;
	// Etwas überhang einplanen falls die wir mal zu schnell laufen
	if(uhrzeit.s >60)
	{
		uhrzeit.s = 1;
		uhrzeit.m++;
		if(uhrzeit.m >= 60)
		{
			uhrzeit.m = 0;
			uhrzeit.h++;
			if(uhrzeit.h >= 24)
			{
				uhrzeit.d++;
				// Auf weitere Stufen verichten wir;
			}
		}
	}
}
