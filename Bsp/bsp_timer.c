//工程设定时钟计时
#include "stm32f10x.h"

volatile uint32_t currentTick=0;

void My_Timer_Init(void)
{

	
	//开启定时器3的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//配置时基单元参数
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	
	TIM_TimeBaseInitStruct.TIM_Prescaler =71;
	TIM_TimeBaseInitStruct.TIM_Period =999;
	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter =0;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	//闭合时基单元开关
	TIM_Cmd(TIM3,ENABLE);
	
	//使能update中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	//配置NVIC模块
	// 中断优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	// NVIC参数
	NVIC_InitTypeDef NVIC_InitStruct={0};
	
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =1;
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE ;
	
	NVIC_Init(&NVIC_InitStruct);
	
	
	
}

//中断累计计时
void TIM3_IRQHandler (void)
{

	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)==SET)
	{
	
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);//清除标志
		currentTick++;
	
	
	}

}	



