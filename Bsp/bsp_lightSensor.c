#include "stm32f10x.h"

uint16_t Light_Data[4]={0};//设立数组接收光敏传感器数据
volatile float LeftUp=0,LeftDown=0,RightUp=0,RightDown=0;//四光敏矩阵模块数据
volatile float Light_Yaw_Angle=90;
volatile float Light_Pitch_Angle=90;
//volatile float total_LightData1=0;
volatile float Light_Yaw1=0;
volatile float Light_Pitch1=0;


void LightSensor_Init(void)
{
	//初始化引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct={0};
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	
	
	//初始化和配置ADC1
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1,ENABLE);

	RCC_ADCCLKConfig (RCC_PCLK2_Div6 );//选择6分频
	
	
	
	ADC_InitTypeDef ADC_InitStruct={0};
	
	ADC_InitStruct.ADC_Mode =ADC_Mode_Independent ;//选择独立模式
	ADC_InitStruct.ADC_DataAlign =ADC_DataAlign_Right ;//右对齐
	ADC_InitStruct.ADC_ExternalTrigConv =ADC_ExternalTrigConv_None ;//软件触发
	ADC_InitStruct.ADC_ContinuousConvMode =ENABLE ;//连续转换
	ADC_InitStruct.ADC_ScanConvMode =ENABLE ;//扫描模式
	ADC_InitStruct.ADC_NbrOfChannel =4;//通道数4
	
	ADC_Init (ADC1,&ADC_InitStruct);
	
	
	//规则组通道配置
	ADC_RegularChannelConfig (ADC1,ADC_Channel_3,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig (ADC1,ADC_Channel_4,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig (ADC1,ADC_Channel_5,3,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig (ADC1,ADC_Channel_6,4,ADC_SampleTime_55Cycles5);
	
	//初始化DMA
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitTypeDef DMA_InitStruct={0};
	
	DMA_InitStruct.DMA_PeripheralBaseAddr =(uint32_t)&ADC1->DR ;//外设基地址ADC->DR 
	DMA_InitStruct.DMA_PeripheralDataSize =DMA_PeripheralDataSize_HalfWord ;//外设数据宽度，半字
	DMA_InitStruct.DMA_PeripheralInc =DMA_PeripheralInc_Disable ;//外设地址不自增，ADC数据寄存器始终为源
	DMA_InitStruct.DMA_MemoryBaseAddr =(uint32_t)Light_Data;//存储器基地址，以全局数组来存放
	DMA_InitStruct.DMA_MemoryDataSize =DMA_MemoryDataSize_HalfWord ;//存储器数据宽度，半字，对应
	DMA_InitStruct.DMA_MemoryInc =DMA_MemoryInc_Enable ;//存储器地址自增
	DMA_InitStruct.DMA_DIR =DMA_DIR_PeripheralSRC ;//数据传输方向，由外设到寄存器即ADC数据寄存器到数组
	DMA_InitStruct.DMA_BufferSize =4;//转运数据大小->转运次数,与ADC通道数一致
	DMA_InitStruct.DMA_Mode=DMA_Mode_Circular ;//循环模式
	DMA_InitStruct.DMA_M2M =DMA_M2M_Disable ;
	DMA_InitStruct.DMA_Priority =DMA_Priority_Medium ;//只有一个外设在使用DMA,优先级都可以，选择中等
	
	
	DMA_Init (DMA1_Channel1,&DMA_InitStruct);
	
	DMA_Cmd (DMA1_Channel1,ENABLE);
	ADC_DMACmd (ADC1,ENABLE);
	ADC_Cmd (ADC1,ENABLE);
	
	//ADC校准
	ADC_ResetCalibration (ADC1);
	while(ADC_GetResetCalibrationStatus (ADC1)==SET);
	ADC_StartCalibration (ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==SET);
	
	ADC_SoftwareStartConvCmd (ADC1,ENABLE);
	

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



