#include"Remote_ship.h"

static uint8_t t;//������� ����
static uint8_t *buffer;//���������� ��������� �� ������ ��� ������/������ SPI
uint8_t size_buffer;//������ ������

//���������� ��������� �� ������ SPI,������� � ������� �����
void spi_buf_init(uint8_t *buf,uint8_t size)
{
	buffer=buf;
	size_buffer=size;
}


// ������������� ����������,����������� ��������� �� ������ � ������� �����
void spi_init(void)
{
  	SPI_DDR |= (1 << SPI_MOSI) | (1 <<  SPI_SCK) | (1 << SPI_SS);
  	SPCR = (1 << SPE) | (1 << MSTR);// ����� 0, ������, ������� 1/4 �� ������� ��
}

// ������� � ��������� 1 ���� �� SPI, ���������� ���������� ��������
uint8_t spi_send_recv(uint8_t data) {
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}
//������� � ��������� ���� COUNT �� SPI,��������� �������� � ������ �� ���������
void spi_run(uint8_t *buf,uint8_t count)
{
	SPDR=*(buf+count);//������ ����(�� �� � ��������� ��������)
}


void spi_stop(void)//��������� ��������
{
	SPCR=0;	
}

ISR(SPI_STC_vect)
{
	rxbuf[t]=SPDR;//������ spi
	t++;//��������� ����
	if(t==SPI_SIZE) 
		{
			spi_stop();
			return;
		};
	SPDR=txbuf[t];//���������� ��������� ���� � ��������� spi
	DDRB=255;//��� �������
	PORTB=SPCR;//��� �������
}



