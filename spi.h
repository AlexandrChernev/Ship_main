#ifndef SPI
#define SPI 1

#include"Remote_ship.h"

#define SPI_DDR DDRB
#define SPI_SS 2
#define SPI_MOSI 3
#define SPI_SCK 5
#define SPI_SIZE 32//размер буфера приёма/передачи


volatile static uint8_t rxbuf[SPI_SIZE];
volatile static uint8_t txbuf[SPI_SIZE]={4,5,2,56,53,44,90,12};

// Инициализация интерфейса
void spi_init(void);
// Передаёт и принимает 1 байт по SPI, возвращает полученное значение
uint8_t spi_send_recv(uint8_t);
void spi_run(void);
//Передаёт и принимает байт COUNT по SPI,записывая значение в массив по указателю
void spi_run(uint8_t* buf,uint8_t count);
void spi_stop(void);
ISR(SPI_STC_vect);

#endif
