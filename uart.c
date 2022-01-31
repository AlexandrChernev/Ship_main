#include"Remote_ship.h"
#include<avr/io.h>

//инициализаци€ UART
void usart_init( uint16_t speed)
{
	UCSR0B=(1<<RXEN0)|( 1<<TXEN0);//вкл. TX и RX режим
	//UCSR0B |= (1<<RXCIE0);//enable interrupt
	UCSR0A |= (1<<U2X0); // если 8ћ√ц
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);//обращение к регистру UCSRC(URSEL=1)
	//асинронный режим (UMSEL=0),без контрол€ чЄтности (UPM1=0 и UPM0=0),
  //1 стоп-бит (USBS=0), 8-бит данных (UCSZ1=1 и UCSZ0=1)
  //UCSRC |= (1<<UPM1);//чЄтность
	UBRR0H = (uint8_t)(speed>>8);
	UBRR0L = (uint8_t)speed;
}

//передаЄм байт
void usart_tx_char(uint8_t data )
{
	while ( !(UCSR0A & (1<<UDRE0)) );
	PAUSE_US(16000);
	UDR0 = data;
}

void usart_tx_line(uint8_t*str)
{
	while(*str)
	{
		usart_tx_char(*(str++));
	}
}

void usart_tx_line_long(uint8_t*str)
{
	while(*str)
	{
		usart_tx_char(*(str++));
		PAUSE_US(16380);
	}
}

void usart_tx_line_long_cr(uint8_t*str)
{
	while(*str)
	{
		usart_tx_char(*(str++));
		PAUSE_US(16380);
	}
	usart_tx_char(0x0d);
	usart_tx_char(0x0a);
}

//принимаем байт
void usart_rx( uint8_t data )
{
}

ISR(USART_RX_vect)
{
}

ISR(USART_TX_vect)
{
}

