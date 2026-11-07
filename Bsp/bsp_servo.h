#ifndef _BSP_SERVO_H_
#define _BSP_SERVO_H_

#include "stm32f10x.h"


//舵机运动标志位
extern volatile  uint8_t  servo_PitchCmd;
extern  volatile  uint8_t  servo_YawCmd;

//舵机目标角度
extern  volatile  uint16_t Pitch_Angle;
extern  volatile  uint16_t 	Yaw_Angle;

void My_servo_Init(void);//舵机初始化
void servo_pitch(void);//绕俯仰轴运动
void servo_yaw(void);//绕方位轴运动
void Servo_Move(void);//运动模式与情况判断

#endif
