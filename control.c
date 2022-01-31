#include"remote_ship.h"


#define ADC_RUN  (ADCSRA=(1<<ADATE)|(1<<ADEN)|(1<<ADSC)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2))

typedef enum {CH0=0,CH1,CH2,CH3,CH4,CH5,CH6,CH7} channel;

channel ch;
uint8_t*buffer_adc;
uint8_t count;
uint8_t curr_adc;


void joystick_adc_init(uint8_t*buf)
{
	ch=CH6;
	buffer_adc=buf;
	ADMUX=ch;//вывод ADC0
	//запуск.непрерывного преобр.„астота 1/128. ”казатель на место в массиве
	ADC_RUN;
	
}

void joystick_read(void)
{
}

void adc_read(void)
{
	static uint8_t i;
	//"щЄлкаем" по кругу каналы
	switch(ch)
	{
		case CH6:*(buffer_adc+0)=ADCL;*(buffer_adc+1)=ADCH;ch=CH7;ADMUX=ch;ADC_RUN;break;
		case CH7:*(buffer_adc+2)=ADCL;*(buffer_adc+3)=ADCH;ch=CH3;ADMUX=ch;ADC_RUN;break;
		case CH3:*(buffer_adc+4)=ADCL;*(buffer_adc+5)=ADCH;ch=CH2;ADMUX=ch;ADC_RUN;break;
		case CH2:*(buffer_adc+6)=ADCL;*(buffer_adc+7)=ADCH;ch=CH6;ADMUX=ch;ADC_RUN;break;
	}
}
