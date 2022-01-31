#ifndef _UART
#define _UART 1

#include"Remote_ship.h"

void usart_init( uint16_t);
ISR(USART_RX_vect);
void usart_tx_char(uint8_t);
void usart_rx(uint8_t);
void usart_tx_line(uint8_t*);
void usart_tx_line_long(uint8_t*);
void usart_tx_line_long_cr(uint8_t*);

#endif
