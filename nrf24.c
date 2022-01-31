/*******************************
������ ��������� ���������� 2021
������ 1.0
*******************************/
#include"Remote_ship.h"

uint8_t *buffer;
uint8_t size_buf;



// �������� �������� ��������� (������� �������) �� ����� CE
void rf_assert_ce() {
  RADIO_PORT_CE |= (1 << RADIO_CE); // ��������� �������� ������ �� ����� CE
}

// �������� ���������� ��������� (������ �������) �� ����� CE
void rf_deassert_ce() {
  RADIO_PORT_CE &= ~(1 << RADIO_CE); // ��������� ������� ������ �� ����� CE  
}

// ��������� ������� ��� ������ � csn �� �������������� ������������ � ���� ������, �� ����� �������� static
// �������� �������� ��������� (������ �������) �� ����� CSN
inline static void csn_assert() {
  RADIO_PORT_CSN &= ~(1 << RADIO_CSN); // ��������� ������� ������ �� ����� CSN
}

// �������� ���������� ��������� (������� �������) �� ����� CSN
inline static void csn_deassert() {
  RADIO_PORT_CSN |= (1 << RADIO_CSN); // ��������� �������� ������ �� ����� CSN
}

// �������������� ����� 
void rf_init(uint8_t *buf,uint8_t size) {
	RADIO_DDR_CSN |= (1 << RADIO_CSN); // ����� CSN �� �����
	RADIO_DDR_CE |= (1 << RADIO_CE); // ����� CE �� �����
	RADIO_DDR_IRQ &= ~(1 << RADIO_IRQ); // IRQ - �� ����
	csn_deassert();
	rf_deassert_ce();
	size_buf=size;
	buffer=buf;
}

// ��������� ������� cmd, � ������ count ���� ������, ������� �� � ����� buf, ���������� ������� �������
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

// ��������� ������� cmd, � ������� count ���� ���������� �� ������ buf, ���������� ������� �������
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

// ������ �������� ������������� �������� reg (�� 0 �� 31) � ���������� ���
uint8_t rf_readreg(uint8_t reg) {
	csn_assert();
	spi_send_recv((reg & 31) | R_REGISTER);
	uint8_t answ = spi_send_recv(0xFF);
	csn_deassert();
	return answ;
}

// ���������� �������� ������������� �������� reg (�� 0 �� 31), ���������� ������� �������
uint8_t rf_writereg(uint8_t reg, uint8_t val) {
	csn_assert();
	uint8_t status = spi_send_recv((reg & 31) | W_REGISTER);
	spi_send_recv(val);
	csn_deassert();
	return status;
}

// ������ count ���� �������������� �������� reg (�� 0 �� 31) � ��������� ��� � ����� buf,
// ���������� ������� �������
uint8_t rf_readreg_buf(uint8_t reg, uint8_t * buf, uint8_t count) {
  return rf_read_buf((reg & 31) | R_REGISTER, buf, count);
}

// ���������� count ���� �� ������ buf � ������������� ������� reg (�� 0 �� 31), ���������� ������� �������
uint8_t rf_writereg_buf(uint8_t reg, uint8_t * buf, uint8_t count) {
	return rf_write_buf((reg & 31) | W_REGISTER, buf, count);
}

// ���������� ������ ������ � ������ FIFO ������� ��������
uint8_t rf_read_rx_payload_width()
{
	csn_assert();
	spi_send_recv(R_RX_PL_WID);
	uint8_t answ = spi_send_recv(0xFF);
	csn_deassert();
	return answ;
}

// ��������� �������. ���������� ������� �������
uint8_t rf_cmd(uint8_t cmd)
{
	csn_assert();
	uint8_t status = spi_send_recv(cmd);
	csn_deassert();
	return status;
}

// ���������� 1, ���� �� ����� IRQ �������� (������) �������.
uint8_t rf_is_interrupt() 
{
	return (RADIO_PIN_IRQ & RADIO_IRQ) ? 0 : 1;
}

// ������� ���������� �������������� ��������� ����������. ���������� 1, � ������ ������, 0 � ������ ������
uint8_t rf_start() {
  uint8_t self_addr[] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2}; // ����������� �����
  uint8_t remote_addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7}; // ����� �������� �������
  uint8_t chan = 3; // ����� �����-������ (� ��������� 0 - 125)

  rf_deassert_ce();
  for(uint8_t cnt = 100;;) {
    rf_writereg(CONFIG, (1 << EN_CRC) | (1 << CRCO) | (1 << PRIM_RX)); // ���������� �������
    if (rf_readreg(CONFIG) == ((1 << EN_CRC) | (1 << CRCO) | (1 << PRIM_RX))) 
      break;
    // ���� ��������� �� �� ��� ��������, �� ������ ���� �����-��� ��� ����������������, ���� �� ��������.
    if (!cnt--)
      return 0; // ���� ����� 100 ������� �� ������� �������� ��� �����, �� ������� � �������
    _delay_loop_2(4000);// 1ms
  }

  rf_writereg(EN_AA, (1 << ENAA_P1)); // ��������� ����������������� ������ �� ������ 1
  rf_writereg(EN_RXADDR, (1 << ERX_P0) | (1 << ERX_P1)); // ��������� ������� 0 � 1
  rf_writereg(SETUP_AW, SETUP_AW_5BYTES_ADDRESS); // ����� ����� ������ 5 ����
  rf_writereg(SETUP_RETR, SETUP_RETR_DELAY_4000MKS | SETUP_RETR_NO_RETRANSMIT); 
  rf_writereg(RF_CH, chan); // ����� ���������� ������
  rf_writereg(RF_SETUP, RF_SETUP_1MBPS | RF_SETUP_0DBM); // ����� �������� 1 ����/� � �������� 0dBm
  
  rf_writereg_buf(RX_ADDR_P0, &remote_addr[0], 5); // ������������� �������� �� ����� 0 
  rf_writereg_buf(TX_ADDR, &remote_addr[0], 5);

  rf_writereg_buf(RX_ADDR_P1, &self_addr[0], 5);
  
  rf_writereg(RX_PW_P0, 0);
  rf_writereg(RX_PW_P1, 32); 
  rf_writereg(DYNPD, (1 << DPL_P0) | (1 << DPL_P1)); // ��������� ������������ ����� ��� ������� 0 � 1
  rf_writereg(FEATURE, 0x04); // ���������� ������������ ����� ������ ������

  rf_writereg(CONFIG, (1 << EN_CRC) | (1 << CRCO) | (1 << PWR_UP) | (1 << PRIM_RX)); // ��������� �������
  return (rf_readreg(CONFIG) == ((1 << EN_CRC) | (1 << CRCO) | (1 << PWR_UP) | (1 << PRIM_RX))) ? 1 : 0;
}

// �������� ����� � ������� ��������. 
// buf - ����� � �������, size - ����� ������ (�� 1 �� 32)
uint8_t send_data(uint8_t *buf, uint8_t size) {
  rf_deassert_ce(); // ���� � ������ �����, �� ��������� ��� 
  uint8_t conf = rf_readreg(CONFIG);
  // ���������� ��� PRIM_RX, � �������� ������� ���������� PWR_UP
  uint8_t status = rf_writereg(CONFIG, (conf & ~(1 << PRIM_RX)) | (1 << PWR_UP)); 
  if (status & (1 << TX_FULL_STATUS))  // ���� ������� ����������� ���������, ������������ � �������
    return 1;
  if (!(conf & (1 << PWR_UP))) // ���� ������� �� ���� ��������, �� ���, ���� ���������� ����������
    _delay_loop_2(8000); //2ms
  rf_write_buf(W_TX_PAYLOAD, buf, size); // ������ ������ �� ��������
  rf_assert_ce(); // ������� �� ����� CE ������� � ������ ��������
  _delay_loop_2(60); // ����� ������� 10���, ������ � �������
  rf_deassert_ce();
  return 0;
}

void check_rf() {
  if (!rf_is_interrupt()) // ���� ���������� ���, �� �� �������������
    return;
  uint8_t status = rf_cmd(NOP);
  rf_writereg(STATUS, status); // ������ ������� ������� �������, ��� ����� ������� ���� ����������
  
  if (status & ((1 << TX_DS) | (1 << MAX_RT))) { // ��������� �������� �������, ��� ���,
    if (status & (1 << MAX_RT)) { // ���� ���������� ������������ ����� �������
      rf_cmd(FLUSH_TX); // ������ ��������� ����� �� �������
      on_send_error(); // ������� ����������
    } 
    if (!(rf_readreg(FIFO_STATUS) & (1 << TX_EMPTY))) { // ���� � ������� ����������� ���� ��� ����������
      rf_assert_ce(); // ������� �� ����� CE ������� � ������ ��������
      PAUSE_US(15); // ����� ������� 10���, ������ � �������
      rf_deassert_ce();
    } else {
      uint8_t conf = rf_readreg(CONFIG);
      rf_writereg(CONFIG, conf | (1 << PRIM_RX)); // ������������� ��� PRIM_RX: ����
      rf_assert_ce(); // ������� ������� �� ����� CE ��������� �����-��� � ����� �����
    }
  }
  uint8_t protect = 4; // � ������� FIFO �� ������ ���� ����� 3 �������. ���� ������, ������ ���-�� �� ���
  while (((status & (7 << RX_P_NO)) != (7 << RX_P_NO)) && protect--) { // ���� � ������� ���� �������� �����
    uint8_t l = rf_read_rx_payload_width(); // ����� ����� ������
    if (l > 32) { // ������. ����� ����� ����� ��������
      rf_cmd(FLUSH_RX); 
    } else { 
      uint8_t buf[32]; // ����� ��� ��������� ������
      rf_read_buf(R_RX_PAYLOAD,buffer,l); // ������������ �����
      if ((status & (7 << RX_P_NO)) == (1 << RX_P_NO)) { // ���� datapipe 1 
      }
    }         
    status = rf_cmd(NOP);
  }
}


void rf_rx_start(void)
{
	rf_writereg(CONFIG,(1<<PRIM_RX));
	uint8_t status = rf_cmd(NOP);
	rf_writereg(STATUS, status);//����� �������
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
