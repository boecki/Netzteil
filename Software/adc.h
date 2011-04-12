#ifndef ADC_H
#define ADC_H

void ADC_Init(void);
 
/* ADC Einzelmessung */
uint16_t ADC_Read( uint8_t channel );

/* ADC Mehrfachmessung mit Mittelwertbbildung */
uint16_t ADC_Read_Avg( uint8_t channel, uint8_t average );
#endif
