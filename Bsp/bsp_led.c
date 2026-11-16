

#include "main.h"
#include "delay.h"
#include "bsp_timer.h"
#include "bsp_button.h"

volatile uint8_t sys_checkpoint=0;
volatile uint32_t LED1StartTime=0;
volatile uint32_t LED2StartTime=0;




//上电自检，依次循环点亮
void My_Check_LED (void)
{

	uint8_t round=0;//循环次数
	for(round=0;round<3;round++)
	{
	
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		
		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		
		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
	
	}
	

}


//LED1自检
void My_Check2_LED1 (void)
{			
	
			if(sys_checkpoint==0)//正常常亮
				{
					HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
				}
			else 
				{
					if(currentTick-LED1StartTime>=500)//有故障1Hz闪烁，即完整周期（亮+灭）1s
						{
							if(HAL_GPIO_ReadPin(GPIOA,GPIO_Pin_0)==GPIO_PIN_RESET)
								{
									HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
								}
							else
								{
									HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
								}
					
							LED1StartTime=currentTick;
							
						}
					
				}

}


//LED2指示当前模式
void  My_Mode_CheckLED2(void)
{

switch(sys_Mode)
{
	case 1:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		break;

	case 2:
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		break;
	
	default:
		if(currentTick-LED2StartTime>=500)
			{
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==GPIO_PIN_RESET)
							{
								HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
							}
							else
							{
								HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
							}
					
							LED2StartTime=currentTick;
			}
		
		break;

}
	




}

//LED3收到完整指令翻转一次
void My_LED3_Check (void)
{

	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==GPIO_PIN_SET)
		{
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
		}
	else 
		{
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		}


}

