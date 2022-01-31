/*******************************
Чернев Александр Николаевич 2021
Версия 1.0
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

//режим приёма вкл.
void rf_rx_start(void);

//режим приёма выкл.
void rf_rx_stop(void);

void rf_assert_ce(void);
void rf_deassert_ce(void);

// Инициализирует порты 
void rf_init(uint8_t *,uint8_t);

// Выполняет команду cmd, и читает count байт ответа, помещая их в буфер buf, возвращает регистр статуса
uint8_t rf_read_buf(uint8_t, uint8_t *, uint8_t);

// Выполняет команду cmd, и передаёт count байт параметров из буфера buf, возвращает регистр статуса
uint8_t rf_write_buf(uint8_t, uint8_t *, uint8_t);

// Читает значение однобайтового регистра reg (от 0 до 31) и возвращает его
uint8_t rf_readreg(uint8_t);

// Записывает значение однобайтового регистра reg (от 0 до 31), возвращает регистр статуса
uint8_t rf_writereg(uint8_t, uint8_t);

// Читает count байт многобайтового регистра reg (от 0 до 31) и сохраняет его в буфер buf,
// возвращает регистр статуса
uint8_t rf_readreg_buf(uint8_t, uint8_t *, uint8_t);

// Записывает count байт из буфера buf в многобайтовый регистр reg (от 0 до 31), возвращает регистр статуса
uint8_t rf_writereg_buf(uint8_t, uint8_t *, uint8_t);

// Возвращает размер данных в начале FIFO очереди приёмника
uint8_t rf_read_rx_payload_width(void);

// Выполняет команду. Возвращает регистр статуса
uint8_t rf_cmd(uint8_t);

// Возвращает 1, если на линии IRQ активный (низкий) уровень.
uint8_t rf_is_interrupt(void);

//инициализация модуля NRF24
uint8_t rf_start(void);
//отправка массива. 1-ок,2-ошибка
uint8_t send_data(uint8_t *, uint8_t);
void on_send_error(void);//обработка ошибки числа попыток передачи
void check_rf(void);

#endif
