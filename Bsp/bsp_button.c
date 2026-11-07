#include "stm32f10x.h"
#include "bsp_timer.h"




volatile uint8_t sys_Mode=1;//当前模式
volatile uint8_t sys_LastMode=1;//上一个模式
volatile uint32_t My_buttonStartTime=0;//按下按钮的时间点

volatile uint8_t My_button_Previous=1;//按钮变化前一刻的状态
volatile uint8_t My_button_Current=1;//按钮现在的状态
volatile uint32_t My_button_duration=0;//按下按钮的持续时间
volatile uint8_t My_button_State=0;//OLED显示按键类型判断点


//初始化PB0引脚
void My_button_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct={0};
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_0;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);


}


//模式转换
void My_button_Press(void)
{

	My_button_Current=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
	
	if(My_button_Current==RESET&&My_button_Previous==SET)//判断按钮是否被按下
		{
			
			My_button_Previous=My_button_Current;//更新状态
			My_buttonStartTime=currentTick;//记录起始时间点
			sys_LastMode=sys_Mode;//记录转换前的模式
		}	
		
		
		
	if(My_button_Current==SET&&My_button_Previous==RESET)//判断按钮是否松开
		{
			My_button_duration=currentTick-My_buttonStartTime;//计算按按钮的持续时间
			
			//判断模式
			if(My_button_duration<50)
				{
				
				}
			else if(My_button_duration>=50&&My_button_duration<1000)
				{
					My_button_State=1;
					
					if(sys_Mode==3)
						{	
							sys_Mode=1;
						}
					else
						{
							sys_Mode++;
						}
					
						}
			else
				{
					My_button_State=2;
					
					if(sys_Mode==1)
						{
							sys_Mode=3;
						
						}
					else 
						{
							sys_Mode--;
						}
							
				}		
					
		}
			
		My_button_Previous=My_button_Current;// 同步状态

}


