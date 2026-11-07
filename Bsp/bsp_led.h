#ifndef _BSP_LED_H_
#define _BSP_LED_H_

#include "stm32f10x.h"

void My_LED_Init(void);//LED初始化
void My_Check_LED (void);//上电自检
void My_Check2_LED1 (void);//LED1自检
extern volatile uint8_t sys_checkpoint;//外设检查点
void My_Mode_CheckLED2(void);//LED2模式指示
void My_LED3_Check (void);//LED3翻转指示


#endif

