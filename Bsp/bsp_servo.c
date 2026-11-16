
#include "main.h"
#include "bsp_usart.h"
#include "bsp_mpu6050.h"
#include "bsp_button.h"
#include "bsp_led.h"
#include "bsp_lightSensor.h"
#include "Delay.h"


volatile  uint8_t  servo_PitchCmd=0;//舵机俯仰运动启动标志位
volatile  uint8_t  servo_YawCmd=0;//舵机转向运动启动标志位
volatile  int32_t  Pitch_Angle=90;//舵机俯仰角度
volatile  int32_t  Yaw_Angle=90;//舵机转向角度
//volatile float LeftUp=0,LeftDown=0,RightUp=0,RightDown=0;//四光敏矩阵模块数据
volatile uint8_t last_Mode=1;//切回模式1回归中位判断点
//volatile float En_LeftUp=0,En_LeftDown=0,En_RightUp=0,En_RightDown=0;
//uint8_t tracking_EnOK=0;



void My_servo_Init(void)
{

	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	
}



void servo_pitch(void)//俯仰
{
	if(servo_PitchCmd==1)//判断
		{
			_HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,Pitch_Angle*2000/180+500);//计算角度占180的比例，再等比例换算成PWM时间，最后加500计数单位满足初始位 
			servo_PitchCmd=0;//重置标志位
		}
		
}

void servo_yaw(void)
{
	if(servo_YawCmd==1)
		{
			_HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,Yaw_Angle*2000/180+500); 
			servo_YawCmd=0;
		}
		
}





//舵机运动
void Servo_Move(void)
{
	
//每切换到模式1则自动回归中位
	if(sys_Mode ==1&&last_Mode!=1)
		{
			Pitch_Angle=90;
			Yaw_Angle=90;
			temp_Pitch=90;
      temp_Yaw=90;
			servo_PitchCmd =1;
			servo_YawCmd =1;
		
		}
		
	if(sys_Mode ==3&&last_Mode!=3)
		{
			Pitch_Angle=90;
			Yaw_Angle=90;
			
			Light_Pitch_Angle =90;
			Light_Yaw_Angle =90;
			servo_PitchCmd =1;
			servo_YawCmd =1;
			
			
		}
	
	last_Mode=sys_Mode;
	
		
		
//判断模式		
	if(sys_Mode ==1)
		{			

			servo_PitchCmd =1;
			servo_YawCmd =1;

		}
	else if(sys_Mode ==2)
		{
			
			if(sys_checkpoint==1)//判断是否出错
				{
					sys_Mode =1;//返回模式一
					return ;
				}
							
			//使MPU范围的-90~90对应舵机的0~180
			Pitch_Angle =MPU_Pitch+90.0;
			Yaw_Angle =-MPU_ROLL +90.0;
				
			
			//标志位置1
			servo_PitchCmd =1;
			servo_YawCmd =1;
				
		
		}
	else
		{
			
			
			
			LightSensor_ServoAngle();
			
			
			Pitch_Angle =Light_Pitch_Angle;
			Yaw_Angle =Light_Yaw_Angle ;
								
					
				servo_PitchCmd =1;
				servo_YawCmd =1;

					
		}
		
		//总判断越界情况	
		if(Pitch_Angle>180)
				{
					Pitch_Angle =180;
				}
			if(Pitch_Angle<0)
				{
					Pitch_Angle =0;
				}	
			if(Yaw_Angle>180)
				{
					Yaw_Angle =180;
				}
			if(Yaw_Angle<0)
				{
					Yaw_Angle =0;
				}		
		
				
		servo_pitch();//俯仰
		servo_yaw();

		sys_LastMode=sys_Mode;//更新模式		
				
}





