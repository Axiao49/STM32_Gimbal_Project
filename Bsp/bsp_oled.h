#ifndef _BSP_OLED_H_
#define _BSP_OLED_H_

#include "stm32f10x.h"

uint8_t  My_oled_Init(void);//初始化
void My_oled_printf(void);//显示
extern volatile uint8_t  My_MPUERR_OLED;//判断MPU是否初始化成功并显示在OLED上
extern volatile uint8_t  My_MPUData_OLED;//判断MPU是否读取数据成功并显示在OLED上


#endif
