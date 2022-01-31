/*******************************
������ ��������� ���������� 2021
������ 1.0
*******************************/
#ifndef NRF24
#define NRF24 1
#include"Remote_ship.h"

#define RADIO_PORT_CE PORTC
#define RADIO_DDR_CE DDRC
#define RADIO_PIN_CE PINC
#define RADIO_PORT_CSN PORTB
#define RADIO_DDR_CSN DDRB
#define RADIO_PIN_CSN PINB
#define RADIO_PORT_IRQ PORTC
#define RADIO_DDR_IRQ DDRC
#define RADIO_PIN_IRQ PINC

#define RADIO_CSN 2
#define RADIO_CE 0
#define RADIO_IRQ 1

//����� ����� ���.
void rf_rx_start(void);

//����� ����� ����.
void rf_rx_stop(void);

void rf_assert_ce(void);
void rf_deassert_ce(void);

// �������������� ����� 
void rf_init(uint8_t *,uint8_t);

// ��������� ������� cmd, � ������ count ���� ������, ������� �� � ����� buf, ���������� ������� �������
uint8_t rf_read_buf(uint8_t, uint8_t *, uint8_t);

// ��������� ������� cmd, � ������� count ���� ���������� �� ������ buf, ���������� ������� �������
uint8_t rf_write_buf(uint8_t, uint8_t *, uint8_t);

// ������ �������� ������������� �������� reg (�� 0 �� 31) � ���������� ���
uint8_t rf_readreg(uint8_t);

// ���������� �������� ������������� �������� reg (�� 0 �� 31), ���������� ������� �������
uint8_t rf_writereg(uint8_t, uint8_t);

// ������ count ���� �������������� �������� reg (�� 0 �� 31) � ��������� ��� � ����� buf,
// ���������� ������� �������
uint8_t rf_readreg_buf(uint8_t, uint8_t *, uint8_t);

// ���������� count ���� �� ������ buf � ������������� ������� reg (�� 0 �� 31), ���������� ������� �������
uint8_t rf_writereg_buf(uint8_t, uint8_t *, uint8_t);

// ���������� ������ ������ � ������ FIFO ������� ��������
uint8_t rf_read_rx_payload_width(void);

// ��������� �������. ���������� ������� �������
uint8_t rf_cmd(uint8_t);

// ���������� 1, ���� �� ����� IRQ �������� (������) �������.
uint8_t rf_is_interrupt(void);

//������������� ������ NRF24
uint8_t rf_start(void);
//�������� �������. 1-��,2-������
uint8_t send_data(uint8_t *, uint8_t);
void on_send_error(void);//��������� ������ ����� ������� ��������
void check_rf(void);

#endif
