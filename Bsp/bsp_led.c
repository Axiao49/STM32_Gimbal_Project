

#include "stm32f10x.h"
#include "delay.h"
#include "bsp_timer.h"
#include "bsp_button.h"

void My_Timer_Init(void);
volatile uint8_t sys_checkpoint=0;
volatile uint32_t LED1StartTime=0;
volatile uint32_t LED2StartTime=0;


void My_LED_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct={0};
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);

}



//上电自检，依次循环点亮
void My_Check_LED (void)
{

	uint8_t round=0;//循环次数
	for(round=0;round<3;round++)
	{
	
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
		Delay(200);
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
		
		
		GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);
		Delay(200);
		GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
		
		
		GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_SET);
		Delay(200);
		GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_RESET);
	
	}
	

}


//LED1自检
void My_Check2_LED1 (void)
{			
	
			if(sys_checkpoint==0)//正常常亮
				{
					GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
				}
			else 
				{
					if(currentTick-LED1StartTime>=500)//有故障1Hz闪烁，即完整周期（亮+灭）1s
						{
							if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_0)==0)
								{
									GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
								}
							else
								{
									GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
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
		GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
		break;

	case 2:
		GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);
		break;
	
	default:
		if(currentTick-LED2StartTime>=500)
			{
				if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1)==0)
							{
								GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
							}
							else
							{
								GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
							}
					
							LED2StartTime=currentTick;
			}
		
		break;

}
	




}

//LED3收到完整指令翻转一次
void My_LED3_Check (void)
{

	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==Bit_SET)
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_RESET);
		}
	else 
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_SET);
		}


}

