#ifndef _BSP_SI2CMPU_H_
#define _BSP_SI2CMPU_H_

#include "stm32f10x.h"

uint8_t  My_si2cMPU_Init(void);
int My_si2cMPU_Send(uint8_t RegAddr,uint8_t Data);//初始化MPU6050专用
int My_si2cMPU_ReReg(uint8_t RegAddr);//读取MPU数据

#endif


