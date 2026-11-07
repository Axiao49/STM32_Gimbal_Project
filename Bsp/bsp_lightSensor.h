#ifndef _BSP_LIGHTSENSOR_H_
#define _BSP_LIGHTSENSOR_H_

#include "stm32f10x.h"

extern uint16_t Light_Data[4];//数据记录
void LightSensor_Init(void);//初始化
void LightSensor_ServoAngle(void);//光强数据计算角度
extern volatile float Light_Yaw_Angle;
extern volatile float Light_Pitch_Angle;
//extern volatile float total_LightData1;
extern volatile float Light_Yaw1;
extern volatile float Light_Pitch1;




#endif


