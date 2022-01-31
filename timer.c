#include"Remote_ship.h"

volatile uint8_t ip;
volatile uint8_t num_timers;
//������ ���������� �� ������ �������� �������
volatile timer_t *tim;

void timer16_init(uint8_t num,volatile timer_t *ptr)
{
	num_timers=num;
	//��������� TC0
	OCR0A=249;
	TCCR0B=(1<<CS00)|(1<<CS01);//clk 1/64,CTC
	TCCR0A=(1<<WGM01);
	TIMSK0|=(1<<OCIE0A);
	tim=ptr;
}

void timer_on(uint16_t period,uint8_t num,uint8_t mode)
{
	tim[num].period=period-1;
	tim[num].count=period-1;
	tim[num].mode=mode;
	//tim[num].reset=1;
	//tim[num].run=1;
	tim[num].status=1;
	tim[num].out=0;
}

inline void timer_handler(void)
{
	for(uint8_t i=0;i<num_timers;i++)
	{
		//���� �� �������,�� ��������� ������
		if(tim[i].status==0) {continue;};
		//���� �� ��������,�� ��������� count � ����.������
		if(tim[i].count>0) {tim[i].count--;continue;};
		//���� ����,�� ����� ��������� ������ � ������
		if(tim[i].mode==1) {tim[i].count=tim[i].period;}
			else
			{
				//������ ��������,�������������.
				tim[i].status=0;
				//����.������
			};
		//���������� �����.
		tim[i].out=1;


		/*if(tim[1].out==1)
		{
			tim[1].out=0;
			if(ip==1) {SYSLED_SET;ip=0;continue;}
			SYSLED_RES;
			ip=1;
		}*/
	}
}

ISR(TIMER0_COMPA_vect)
{
	//�������� ����������.�������� �������
	#ifdef TIMER_DIV
		uint8_t i;
		if(i!=(TIMER_DIV-1)) {i++;return;}
		timer_handler();
	#endif

	timer_handler();
}
