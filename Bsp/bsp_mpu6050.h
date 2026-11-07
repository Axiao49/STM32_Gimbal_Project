#ifndef _BSP_MPU6050_H_
#define _BSP_MPU6050_H_

#include "stm32f10x.h"

extern int16_t Accel_X,Accel_Y,Accel_Z;
extern int16_t Gyro_X,Gyro_Y,Gyro_Z;
extern float MPU_Pitch,MPU_ROLL;
uint8_t My_MPU6050_Init(void);
uint8_t  MPU6050_REData(void);
void MPU6050_UpdateAngle(void);
uint8_t MPU6050_AngleCalibration(void);
extern uint8_t Calibration_point;


#endif


