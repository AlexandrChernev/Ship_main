/*******************************
Чернев Александр Николаевич 2021
Версия 1.0
*******************************/
#include"Remote_ship.h"

uint8_t *buffer;
uint8_t size_buf;



// Выбирает активное состояние (высокий уровень) на линии CE
void rf_assert_ce() {
  RADIO_PORT_CE |= (1 << RADIO_CE); // Установка высокого уровня на линии CE
}

// Выбирает неактивное состояние (низкий уровень) на линии CE
void rf_deassert_ce() {
  RADIO_PORT_CE &= ~(1 << RADIO_CE); // Установка низкого уровня на линии CE  
}

// Поскольку функции для работы с csn не предполагается использовать в иных файлах, их можно объявить static
// Выбирает активное состояние (низкий уровень) на линии CSN
inline static void csn_assert() {
  RADIO_PORT_CSN &= ~(1 << RADIO_CSN); // Установка низкого уровня на линии CSN
}

// Выбирает неактивное состояние (высокий уровень) на линии CSN
inline static void csn_deassert() {
  RADIO_PORT_CSN |= (1 << RADIO_CSN); // Установка высокого уровня на линии CSN
}

// Инициализирует порты 
void rf_init(uint8_t *buf,uint8_t size) {
	RADIO_DDR_CSN |= (1 << RADIO_CSN); // Ножка CSN на выход
	RADIO_DDR_CE |= (1 << RADIO_CE); // Ножка CE на выход
	RADIO_DDR_IRQ &= ~(1 << RADIO_IRQ); // IRQ - на вход
	csn_deassert();
	rf_deassert_ce();
	size_buf=size;
	buffer=buf;
}

// Выполняет команду cmd, и читает count байт ответа, помещая их в буфер buf, возвращает регистр статуса
uint8_t rf_read_buf(uint8_t cmd, uint8_t * buf, uint8_t count) {
	csn_assert();
	uint8_t status = spi_send_recv(cmd);
	while (count--) 
	{
		*(buf++) = spi_send_recv(0xFF);
	}
	csn_deassert();
	return status;
}

// Выполняет команду cmd, и передаёт count байт параметров из буфера buf, возвращает регистр статуса
uint8_t rf_write_buf(uint8_t cmd, uint8_t * buf, uint8_t count)
{
	csn_assert();
	uint8_t status = spi_send_recv(cmd);
	while (count--)
	{
		spi_send_recv(*(buf++));
	}
	csn_deassert();
	return status;
}

// Читает значение однобайтового регистра reg (от 0 до 31) и возвращает его
uint8_t rf_readreg(uint8_t reg) {
	csn_assert();
	spi_send_recv((reg & 31) | R_REGISTER);
	uint8_t answ = spi_send_recv(0xFF);
	csn_deassert();
	return answ;
}

// Записывает значение однобайтового регистра reg (от 0 до 31), возвращает регистр статуса
uint8_t rf_writereg(uint8_t reg, uint8_t val) {
	csn_assert();
	uint8_t status = spi_send_recv((reg & 31) | W_REGISTER);
	spi_send_recv(val);
	csn_deassert();
	return status;
}

// Читает count байт многобайтового регистра reg (от 0 до 31) и сохраняет его в буфер buf,
// возвращает регистр статуса
uint8_t rf_readreg_buf(uint8_t reg, uint8_t * buf, uint8_t count) {
  return rf_read_buf((reg & 31) | R_REGISTER, buf, count);
}

// Записывает count байт из буфера buf в многобайтовый регистр reg (от 0 до 31), возвращает регистр статуса
uint8_t rf_writereg_buf(uint8_t reg, uint8_t * buf, uint8_t count) {
	return rf_write_buf((reg & 31) | W_REGISTER, buf, count);
}

// Возвращает размер данных в начале FIFO очереди приёмника
uint8_t rf_read_rx_payload_width()
{
	csn_assert();
	spi_send_recv(R_RX_PL_WID);
	uint8_t answ = spi_send_recv(0xFF);
	csn_deassert();
	return answ;
}

// Выполняет команду. Возвращает регистр статуса
uint8_t rf_cmd(uint8_t cmd)
{
	csn_assert();
	uint8_t status = spi_send_recv(cmd);
	csn_deassert();
	return status;
}

// Возвращает 1, если на линии IRQ активный (низкий) уровень.
uint8_t rf_is_interrupt() 
{
	return (RADIO_PIN_IRQ & RADIO_IRQ) ? 0 : 1;
}

// Функция производит первоначальную настройку устройства. Возвращает 1, в случае успеха, 0 в случае ошибки
uint8_t rf_start() {
  uint8_t self_addr[] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2}; // Собственный адрес
  uint8_t remote_addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7}; // Адрес удалённой стороны
  uint8_t chan = 3; // Номер радио-канала (в диапазоне 0 - 125)

  rf_deassert_ce();
  for(uint8_t cnt = 100;;) {
    rf_writereg(CONFIG, (1 << EN_CRC) | (1 << CRCO) | (1 << PRIM_RX)); // Выключение питания
    if (rf_readreg(CONFIG) == ((1 << EN_CRC) | (1 << CRCO) | (1 << PRIM_RX))) 
      break;
    // Если прочитано не то что записано, то значит либо радио-чип ещё инициализируется, либо не работает.
    if (!cnt--)
      return 0; // Если после 100 попыток не удалось записать что нужно, то выходим с ошибкой
    _delay_loop_2(4000);// 1ms
  }

  rf_writereg(EN_AA, (1 << ENAA_P1)); // включение автоподтверждения только по каналу 1
  rf_writereg(EN_RXADDR, (1 << ERX_P0) | (1 << ERX_P1)); // включение каналов 0 и 1
  rf_writereg(SETUP_AW, SETUP_AW_5BYTES_ADDRESS); // выбор длины адреса 5 байт
  rf_writereg(SETUP_RETR, SETUP_RETR_DELAY_4000MKS | SETUP_RETR_NO_RETRANSMIT); 
  rf_writereg(RF_CH, chan); // Выбор частотного канала
  rf_writereg(RF_SETUP, RF_SETUP_1MBPS | RF_SETUP_0DBM); // выбор скорости 1 Мбит/с и мощности 0dBm
  
  rf_writereg_buf(RX_ADDR_P0, &remote_addr[0], 5); // Подтверждения приходят на канал 0 
  rf_writereg_buf(TX_ADDR, &remote_addr[0], 5);

  rf_writereg_buf(RX_ADDR_P1, &self_addr[0], 5);
  
  rf_writereg(RX_PW_P0, 0);
  rf_writereg(RX_PW_P1, 32); 
  rf_writereg(DYNPD, (1 << DPL_P0) | (1 << DPL_P1)); // включение произвольной длины для каналов 0 и 1
  rf_writereg(FEATURE, 0x04); // разрешение произвольной длины пакета данных

  rf_writereg(CONFIG, (1 << EN_CRC) | (1 << CRCO) | (1 << PWR_UP) | (1 << PRIM_RX)); // Включение питания
  return (rf_readreg(CONFIG) == ((1 << EN_CRC) | (1 << CRCO) | (1 << PWR_UP) | (1 << PRIM_RX))) ? 1 : 0;
}

// Помещает пакет в очередь отправки. 
// buf - буфер с данными, size - длина данных (от 1 до 32)
uint8_t send_data(uint8_t *buf, uint8_t size) {
  rf_deassert_ce(); // Если в режиме приёма, то выключаем его 
  uint8_t conf = rf_readreg(CONFIG);
  // Сбрасываем бит PRIM_RX, и включаем питание установкой PWR_UP
  uint8_t status = rf_writereg(CONFIG, (conf & ~(1 << PRIM_RX)) | (1 << PWR_UP)); 
  if (status & (1 << TX_FULL_STATUS))  // Если очередь передатчика заполнена, возвращаемся с ошибкой
    return 1;
  if (!(conf & (1 << PWR_UP))) // Если питание не было включено, то ждём, пока запустится осциллятор
    _delay_loop_2(8000); //2ms
  rf_write_buf(W_TX_PAYLOAD, buf, size); // Запись данных на отправку
  rf_assert_ce(); // Импульс на линии CE приведёт к началу передачи
  _delay_loop_2(60); // Нужно минимум 10мкс, возьмём с запасом
  rf_deassert_ce();
  return 0;
}

void check_rf() {
  if (!rf_is_interrupt()) // Если прерывания нет, то не задерживаемся
    return;
  uint8_t status = rf_cmd(NOP);
  rf_writereg(STATUS, status); // Просто запишем регистр обратно, тем самым сбросив биты прерываний
  
  if (status & ((1 << TX_DS) | (1 << MAX_RT))) { // Завершена передача успехом, или нет,
    if (status & (1 << MAX_RT)) { // Если достигнуто максимальное число попыток
      rf_cmd(FLUSH_TX); // Удалим последний пакет из очереди
      on_send_error(); // Вызовем обработчик
    } 
    if (!(rf_readreg(FIFO_STATUS) & (1 << TX_EMPTY))) { // Если в очереди передатчика есть что передавать
      rf_assert_ce(); // Импульс на линии CE приведёт к началу передачи
      PAUSE_US(15); // Нужно минимум 10мкс, возьмём с запасом
      rf_deassert_ce();
    } else {
      uint8_t conf = rf_readreg(CONFIG);
      rf_writereg(CONFIG, conf | (1 << PRIM_RX)); // Устанавливаем бит PRIM_RX: приём
      rf_assert_ce(); // Высокий уровень на линии CE переводит радио-чип в режим приёма
    }
  }
  uint8_t protect = 4; // В очереди FIFO не должно быть более 3 пакетов. Если больше, значит что-то не так
  while (((status & (7 << RX_P_NO)) != (7 << RX_P_NO)) && protect--) { // Пока в очереди есть принятый пакет
    uint8_t l = rf_read_rx_payload_width(); // Узнаём длину пакета
    if (l > 32) { // Ошибка. Такой пакет нужно сбросить
      rf_cmd(FLUSH_RX); 
    } else { 
      uint8_t buf[32]; // буфер для принятого пакета
      rf_read_buf(R_RX_PAYLOAD,buffer,l); // начитывается пакет
      if ((status & (7 << RX_P_NO)) == (1 << RX_P_NO)) { // если datapipe 1 
      }
    }         
    status = rf_cmd(NOP);
  }
}


void rf_rx_start(void)
{
	rf_writereg(CONFIG,(1<<PRIM_RX));
	uint8_t status = rf_cmd(NOP);
	rf_writereg(STATUS, status);//сброс статуса
	rf_assert_ce();
}

void rf_rx_stop(void)
{
	rf_deassert_ce();
	uint8_t conf=rf_readreg(CONFIG);
	rf_writereg(CONFIG,conf&~(1<<PRIM_RX));

}

void on_send_error(void)
{
}
