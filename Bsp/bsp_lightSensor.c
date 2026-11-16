#include "main.h"

volatile uint16_t Light_Data[4]={0};//设立数组接收光敏传感器数据
volatile float LeftUp=0,LeftDown=0,RightUp=0,RightDown=0;//四光敏矩阵模块数据
volatile float Light_Yaw_Angle=90;
volatile float Light_Pitch_Angle=90;
//volatile float total_LightData1=0;
volatile float Light_Yaw1=0;
volatile float Light_Pitch1=0;


void LightSensor_Init(void)
{
	//ADC校准
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)Light_Data,4);

}

void LightSensor_ServoAngle(void)
{

			int16_t Raw_RightUp=4095-Light_Data[0];//A1
			int16_t Raw_RightDown=4095-Light_Data[1];//A2
			int16_t Raw_LeftDown=4095-Light_Data[2];//A3
			int16_t Raw_LeftUp=4095-Light_Data[3];//A4
				
				
			LeftUp=LeftUp*0.7f+(float)Raw_LeftUp*0.3f;//赋权滤去突变的毛刺同时能较稳定迅速更新数值
			LeftDown=LeftDown*0.7f+(float)Raw_LeftDown*0.3f;
			RightUp=RightUp*0.7f+(float)Raw_RightUp*0.3f;
			RightDown=RightDown*0.7f+(float)Raw_RightDown*0.3f;
				
			float total_LightData= LeftUp+LeftDown+RightUp+RightDown;				
//    	total_LightData1=total_LightData;

	    if(total_LightData>=400)//经检测，当光强总和大于此值时，可视为有显著光源
			{
					
			
				float Light_Yaw=(LeftUp+LeftDown)-(RightUp+RightDown);
				float Light_Pitch=(LeftUp+RightUp)-(LeftDown+RightDown);
					
				Light_Yaw1=Light_Yaw;
				Light_Pitch1=Light_Pitch;
				
				float step=2;
				int32_t Turn_line=80;//
						
				
				
				if(Light_Yaw>Turn_line)//左边更亮，左转
					{
						
						Light_Yaw_Angle-=step ;
						
					}
				else if(Light_Yaw<-Turn_line)//右边更亮，右转
					  {
							
						 Light_Yaw_Angle+=step ;
							
					  }
				 
				
				if(Light_Pitch>Turn_line)//上面更亮，仰高
					  {
						 Light_Pitch_Angle-=step ;
						
					  }
				else if(Light_Pitch<-Turn_line)//下面更亮，俯低
					  {
							
						 Light_Pitch_Angle+=step ;
					  }
				 
				if(Light_Pitch_Angle>180)
				 {
					Light_Pitch_Angle =180;
				 }
			  if(Light_Pitch_Angle<0)
				 {
					Light_Pitch_Angle =0;
				 }	
			  if(Light_Yaw_Angle>180)
				 {
					Light_Yaw_Angle =180;
				 }
			  if(Light_Yaw_Angle<0)
				 {
					Light_Yaw_Angle =0;
				 }				
						
						
				 }					
				
				



}



