#include"remote_ship.h"

volatile timer_t timer[TIMER_NUM];//определяем массив таймеров
uint8_t m_buf[32];
uint8_t p;
uint8_t string0[]="TX buffer is full!";
uint8_t string1[]="TX buffer is EMPTY.Transieve start.OK!";

void init(void)
{
	timer16_init(TIMER_NUM,timer);//запуск аппаратного таймера,передача указателя
	spi_init();
	rf_init(m_buf,32);
	usart_init(UBRR_VAL);
	rf_start();
}
int main(void)
{
	cli();
	init();
	timer_on(200,4,1);
	rf_assert_ce();//активируем,в данном случае,режим приёма
	sei();
	while(1)
	{
		if(timer[4].out)
		{
			volatile uint8_t i=send_data(m_buf,32);
			if(i==1) usart_tx_line(string0);
			if(i==0) usart_tx_line(string1);
			timer[4].out=0;
			if(p){p=0;m_buf[1]=1;}
			else {p=1;m_buf[1]=0;}
		}
		// TODO здесь основной код программы
	}
}
