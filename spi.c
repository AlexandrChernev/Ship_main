#include"Remote_ship.h"

static uint8_t t;//текущий байт
static uint8_t *buffer;//глобальный указатель на массив для чтения/записи SPI
uint8_t size_buffer;//размер буфера

//нацеливаем указатель на массив SPI,лежащий в главном блоке
void spi_buf_init(uint8_t *buf,uint8_t size)
{
	buffer=buf;
	size_buffer=size;
}


// Инициализация интерфейса,нацеливание указателя на массив в главном блоке
void spi_init(void)
{
  	SPI_DDR |= (1 << SPI_MOSI) | (1 <<  SPI_SCK) | (1 << SPI_SS);
  	SPCR = (1 << SPE) | (1 << MSTR);// режим 0, мастер, частота 1/4 от частоты ЦП
}

// Передаёт и принимает 1 байт по SPI, возвращает полученное значение
uint8_t spi_send_recv(uint8_t data) {
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}
//Передаёт и принимает байт COUNT по SPI,записывая значение в массив по указателю
void spi_run(uint8_t *buf,uint8_t count)
{
	SPDR=*(buf+count);//первый байт(он же и запускает передачу)
}


void spi_stop(void)//остановка передачи
{
	SPCR=0;	
}

ISR(SPI_STC_vect)
{
	rxbuf[t]=SPDR;//читаем spi
	t++;//следующий байт
	if(t==SPI_SIZE) 
		{
			spi_stop();
			return;
		};
	SPDR=txbuf[t];//записываем следующий байт и запускаем spi
	DDRB=255;//для отладки
	PORTB=SPCR;//для отладки
}



