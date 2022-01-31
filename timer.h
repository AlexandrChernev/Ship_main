#ifndef TIMER
#define TIMER 1

#include"Remote_ship.h"

typedef struct 
{
	uint16_t count;//���� � ��,��������� ������ ��.
	uint16_t period;//������ � ��
	//uint8_t run;//1-������,�������,0-����������
	uint8_t mode;//1-���������,0-�������������
	//uint8_t reset;//����� ��������
	uint8_t out;//�����
	uint8_t status;//���������,1-���������,0-������
}timer_t;

void timer16_init(uint8_t,volatile timer_t*);//������ ������� �� 0 �� 65536 ��
void timer32_init(void);//������ ������� �� 0 �� 4�294�967�296 ��
inline void timer_handler();//��������� ������� ��������,�������� ����� 1 ��
//������ ����������� �������. 1,-������ 0...255 ��, 3-�����(���� ��� ���������)
void timer_on(uint16_t,uint8_t,uint8_t);
uint8_t millis(void);




#endif
