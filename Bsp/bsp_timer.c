//工程设定时钟计时
#include "main.h"

volatile uint32_t currentTick=0;

void My_Timer_Init(void)
{
  HAL_TIM_Base_Start_IT(&htim3); 
}

//中断累计计时
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
	
		currentTick++;
	
	
	}

}	



