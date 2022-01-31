#ifndef TIMER
#define TIMER 1

#include"Remote_ship.h"

typedef struct 
{
	uint16_t count;//счет в мс,уменбшаем каждую мс.
	uint16_t period;//период в мс
	//uint8_t run;//1-запуск,запущен,0-остановлен
	uint8_t mode;//1-одиночный,0-периодический
	//uint8_t reset;//сброс счетчика
	uint8_t out;//выход
	uint8_t status;//состояние,1-действует,0-пустой
}timer_t;

void timer16_init(uint8_t,volatile timer_t*);//запуск таймера от 0 до 65536 мс
void timer32_init(void);//запуск таймера от 0 до 4 294 967 296 мс
inline void timer_handler();//обработка массива таймеров,вызываем через 1 мс
//запуск конкретного таймера. 1,-период 0...255 мс, 3-режим(авто или одиночный)
void timer_on(uint16_t,uint8_t,uint8_t);
uint8_t millis(void);




#endif
