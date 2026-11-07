#include "stm32f10x.h"
#include "math.h"
#include "delay.h"

#include "bsp_si2c.h"
#include "bsp_led.h"
#include "jka_MPU6050.h"
#include "bsp_si2cMPU.h"
#include "bsp_oled.h"


int16_t Accel_X,Accel_Y,Accel_Z;//记录加速度计X Y Z的原始数据
int16_t Gyro_X,Gyro_Y,Gyro_Z;//记录陀螺仪X Y Z的原始数据 
float MPU_Pitch,MPU_ROLL;//最终结算出的Pitch，Roll
//零位校准
float Gyro_offset_X=0.0f;//陀螺仪X轴静止时的数据（偏差），即零位偏置
float Gyro_offset_Y=0.0f;//陀螺仪Y轴静止时的数据（偏差），即零位偏置
//接收指令
uint8_t Calibration_point=0;//执行姿态零位校准标志位





int8_t My_MPU6050_Init(void)
{
	
	My_si2cMPU_Send (MPU6050_PWR_MGMT_1,0x01);//解除休眠模式，选择X轴陀螺仪作为时钟源
	My_si2cMPU_Send(MPU6050_SMPLRT_DIV,0x09);//采样频率
	My_si2cMPU_Send(MPU6050_CONFIG,0X03);//数字低通滤波，滤除机械抖动
	My_si2cMPU_Send(MPU6050_GYRO_CONFIG,0X08);//角速度量程+-500°/s
	My_si2cMPU_Send(MPU6050_ACCEL_CONFIG,0X00);//重力加速度量程+-2g
	
	return 0;
}



//
//
//读取MPU6050寄存器里的数据
uint8_t MPU6050_REData(void)
{
	uint8_t DataH,DataL;//存放数据的高位/低位部分

	DataH =My_si2cMPU_ReReg(MPU6050_ACCEL_XOUT_H);
	DataL =My_si2cMPU_ReReg(MPU6050_ACCEL_XOUT_L);
	Accel_X=(DataH <<8)|DataL ;//拼接出完整数据
	
	DataH =My_si2cMPU_ReReg(MPU6050_ACCEL_YOUT_H);
	DataL =My_si2cMPU_ReReg(MPU6050_ACCEL_YOUT_L);
	Accel_Y=(DataH <<8)|DataL ;
	
	DataH =My_si2cMPU_ReReg(MPU6050_ACCEL_ZOUT_H);
	DataL =My_si2cMPU_ReReg(MPU6050_ACCEL_ZOUT_L);
	Accel_Z=(DataH <<8)|DataL ;
	
	DataH =My_si2cMPU_ReReg(MPU6050_GYRO_XOUT_H);
	DataL =My_si2cMPU_ReReg(MPU6050_GYRO_XOUT_L);
	Gyro_X=(DataH <<8)|DataL ;
	
	DataH =My_si2cMPU_ReReg(MPU6050_GYRO_YOUT_H);
	DataL =My_si2cMPU_ReReg(MPU6050_GYRO_YOUT_L);
	Gyro_Y=(DataH <<8)|DataL ;
	
	DataH =My_si2cMPU_ReReg(MPU6050_GYRO_ZOUT_H);
	DataL =My_si2cMPU_ReReg(MPU6050_GYRO_ZOUT_L);
	Gyro_Z=(DataH <<8)|DataL ;
	
	return 0;
}

//
//
//零位校准
uint8_t  MPU6050_AngleCalibration(void)
{

	int32_t sum_gx=0;
	int32_t sum_gy=0;
	
	
	//连续采样500次
	for(int i=0;i<500;i++)
		{
			MPU6050_REData();
			sum_gx +=Gyro_X ;
			sum_gy+=Gyro_Y ;
			
			Delay(2);//防止采样过快
	
		}

	Gyro_offset_X=(float)sum_gx /500.0f;
	Gyro_offset_Y=(float)sum_gy /500.0f;
	

	//归零姿态角，定义为0度起点
	MPU_Pitch =0;
	MPU_ROLL =0;

	return 0;
		
}

//
//
//更新，解算姿态角
void MPU6050_UpdateAngle(void)
{	

	
	//读取传感器的数据
	int resultmpuRE=MPU6050_REData ();
	
	if(resultmpuRE!=0)
		{
			sys_checkpoint =1;
		}
	
	
	
	//算俯仰角，横滚角（正交分解+角度换算）
	float acc_Pitch =atan2(-Accel_X,sqrt(Accel_Y*Accel_Y+Accel_Z*Accel_Z)) *180.0f/3.1415926f;//'X'前负号确定芯片坐标轴与设定的坐标轴对应，弧度变角度
	float acc_ROLL=atan2(Accel_Y ,sqrt(Accel_X*Accel_X+Accel_Z*Accel_Z ))*180.0f/3.1415926f;

	//转化成 度/秒
	float gyro_x_rate=(Gyro_X-Gyro_offset_X )/65.5f;//去除偏差
	float gyro_y_rate=(Gyro_Y-Gyro_offset_Y ) /65.5f;
	
	
	//互补滤波融合
	MPU_Pitch=0.8f*(MPU_Pitch +gyro_x_rate *0.02f)+0.2f*acc_Pitch ;
	MPU_ROLL =0.8f*(MPU_ROLL +gyro_y_rate *0.02f)+0.2f*acc_ROLL ;

}










