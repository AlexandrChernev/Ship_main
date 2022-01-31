#ifndef REM_CONTR
#define REM_CONTR 1

#include<avr/interrupt.h>
#include<avr/io.h>
//#include<stdio.h>

#include<util/delay_basic.h>
#include"control.h"
#include"timer.h"
#include"spi.h"
#include"nrf24.h"
#include"io_nrf24.h"
#include"spi.h"
#include"uart.h"

#define DDRLED DDRB
#define PORTLED PORTB
#define PINLED 5
#define F_CPU 16000000U
#define LED_ON1 PORTC|=(1<<0)//
#define LED_ON2 PORTC|=(1<<1)
#define LED_ON3 PORTC|=(1<<2)
#define LED_ON4 PORTC|=(1<<3)
#define LED_OFF1 PORTC&=~(1<<0)//
#define LED_OFF2 PORTC&=~(1<<1)
#define LED_OFF3 PORTC&=~(1<<2)
#define LED_OFF4 PORTC&=~(1<<3)
#define SYSLED_SET PORTLED|=(1<<PINLED);//���.��������� ���������
#define SYSLED_RES (PORTLED&=~(1<<PINLED))//����.��������� ���������
#define SYSLED_INV (PORTLED^=(1<<PINLED))//������������� ��������� ���������
//�����(������ Delay). x-������,��, y-����� �������
#define PAUSE(x,y) timer_on(x,y,0);while((timer[y].out)==0);
//�� ������ DELAY,������� ���
#define PAUSE_US(x) (_delay_loop_2(F_CPU*x/4000000L))
//***************************
//����� ����� � ������� �������,� �������� ������������� ������ ���
//�������� ��� 2 �����,������� �� 4 ������ ���������� 8 ���� �������
#define ADR_ADC 0
//***************************
#define NUMBER_CHANNEL_ADC 2//���-�� ������� ���
//��������� ����� ��� ��� ���������
#define UBRR_VAL 832







#define TIMER_NUM 5//���-��  ����������� ��������
//#define TIMER_DIV 10//�������� ����������.�������� �������

//#define DEBUG 1//�������� ����������.

#endif
