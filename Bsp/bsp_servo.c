
#include "stm32f10x.h"
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

	//初始化引脚PB6 PB7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct={0};
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	
	
	//开启定时器4的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	//配置时基单元参数
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	
	TIM_TimeBaseInitStruct.TIM_Prescaler =71;//psc
	TIM_TimeBaseInitStruct.TIM_Period =19999;//arr
	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter =0;
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	//闭合时基单元开关
	TIM_Cmd(TIM4,ENABLE);
	
	//输出比较
	
	//PB6 PITCH 俯仰
	TIM_OCInitTypeDef TIM_OCInitStruct={0};
	
	TIM_OCInitStruct.TIM_OCMode =TIM_OCMode_PWM1 ;//选择PWM模式1
	TIM_OCInitStruct.TIM_OCPolarity =TIM_OCPolarity_High ;//高电平有效
	TIM_OCInitStruct.TIM_OutputState =TIM_OutputState_Enable ;//使能输出
	TIM_OCInitStruct.TIM_Pulse =1500;//ccr
	
	TIM_OC1Init(TIM4,&TIM_OCInitStruct);//通道1
	
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable );//使能通道1预装载
	TIM_ARRPreloadConfig(TIM4,ENABLE);//使能ARR自动重装预加载
	
	//PB7	YAW左右
	
	TIM_OCInitStruct.TIM_Pulse =1500;//ccr
	TIM_OC2Init(TIM4,&TIM_OCInitStruct);//通道2
	
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable );
	TIM_ARRPreloadConfig(TIM4,ENABLE);
	
	
	TIM_Cmd(TIM4,ENABLE);
	
	
}



void servo_pitch(void)//俯仰
{
	if(servo_PitchCmd	==1)//判断
		{
			TIM_SetCompare1(TIM4,Pitch_Angle*2000/180+500);//计算角度占180的比例，再等比例换算成PWM时间，最后加500计数单位满足初始位 
			servo_PitchCmd=0;//重置标志位
		}
		
}

void servo_yaw(void)
{
	if(servo_YawCmd==1)
		{
			TIM_SetCompare2(TIM4,Yaw_Angle*2000/180+500); 
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





