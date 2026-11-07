#ifndef _BSP_TIMER_H_
#define _BSP_TIMER_H_

#include "stm32f10x.h"

void My_Timer_Init(void);//时钟初始化
extern volatile uint32_t currentTick;//累计计时

#endif


