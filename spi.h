#ifndef SPI
#define SPI 1

#include"Remote_ship.h"

#define SPI_DDR DDRB
#define SPI_SS 2
#define SPI_MOSI 3
#define SPI_SCK 5
#define SPI_SIZE 32//������ ������ �����/��������


volatile static uint8_t rxbuf[SPI_SIZE];
volatile static uint8_t txbuf[SPI_SIZE]={4,5,2,56,53,44,90,12};

// ������������� ����������
void spi_init(void);
// ������� � ��������� 1 ���� �� SPI, ���������� ���������� ��������
uint8_t spi_send_recv(uint8_t);
void spi_run(void);
//������� � ��������� ���� COUNT �� SPI,��������� �������� � ������ �� ���������
void spi_run(uint8_t* buf,uint8_t count);
void spi_stop(void);
ISR(SPI_STC_vect);

#endif
