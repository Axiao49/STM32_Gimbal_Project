#ifndef _BSP_BUTTON_H_
#define _BSP_BUTTON_H_

#include "stm32f10x.h"


void My_button_Init(void);//初始化
extern volatile uint8_t sys_Mode;//当前模式
extern volatile uint8_t sys_LastMode;//上一个模式
extern volatile uint32_t My_button_duration;//按按钮的持续时间
void My_button_Press(void);//模式转换
extern volatile uint8_t My_button_State;


#endif

