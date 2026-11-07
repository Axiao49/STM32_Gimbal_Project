#include "stm32f10x.h"
#include "string.h"


#include "oled.h"
#include "bsp_si2c.h"
#include "bsp_led.h"
#include "bsp_button.h"
#include "bsp_usart.h"
#include "bsp_servo.h"
#include "bsp_mpu6050.h"


OLED_TypeDef oled;
volatile uint8_t  My_MPUERR_OLED=0;//判断MPU是否初始化成功并显示在OLED上
volatile uint8_t  My_MPUData_OLED=0;//判断MPU是否读取数据成功并显示在OLED上


int i2c_write_bytes(uint8_t addr,const uint8_t *pData,uint16_t size)
{

	return My_si2c_SendBytes(addr,(uint8_t *)pData,size);

}



uint8_t  My_oled_Init(void)
{

	
	OLED_InitTypeDef OLED_InitStruct;
	OLED_InitStruct.i2c_write_cb = i2c_write_bytes;

	int resultoled=OLED_Init(&oled,&OLED_InitStruct);
	
//	return resultoled;
	return 0;

}

void My_oled_printf(void)
{

	OLED_Clear(&oled); 
	
	//常显CLOUDHOUSE
	OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
	OLED_SetBrush (&oled,PEN_COLOR_BLACK);
	OLED_SetCursor (&oled,28,8);
	OLED_DrawString (&oled,"CLOUDHOUSE");


	//基本框架.当前模式
	OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
	OLED_SetBrush (&oled,PEN_COLOR_BLACK);
	OLED_SetCursor (&oled,0,20);
	OLED_DrawString (&oled,"MODE:");

	
	//最近一次按键事件类型
	OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
	OLED_SetBrush (&oled,PEN_COLOR_BLACK);
	OLED_SetCursor (&oled,0,32);
	OLED_DrawString (&oled,"BUTTON_EVENT:");


	//最近一次收到的有效指令
	OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
	OLED_SetBrush (&oled,PEN_COLOR_BLACK);
	OLED_SetCursor (&oled,0,44);
	OLED_DrawString (&oled,"COMMAND:");


  if(sys_Mode ==1)
   {
	  //显示舵机俯仰轴的目标角
	  OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
	  OLED_SetBrush (&oled,PEN_COLOR_BLACK);
	  OLED_SetCursor (&oled,0,56);
	  OLED_Printf(&oled,"P:%d",temp_Pitch);
	
	  //显示舵机方位轴的目标角
	  OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
	  OLED_SetBrush (&oled,PEN_COLOR_BLACK);
	  OLED_SetCursor (&oled,40,56);
	  OLED_Printf(&oled,"Y:%d",temp_Yaw);
	 
	 }
	else
	 {
	  OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
	  OLED_SetBrush (&oled,PEN_COLOR_BLACK);
	  OLED_SetCursor (&oled,0,56);
	  OLED_Printf(&oled,"P:%d",Pitch_Angle);
	
	  OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
	  OLED_SetBrush (&oled,PEN_COLOR_BLACK);
	  OLED_SetCursor (&oled,40,56);
	  OLED_Printf(&oled,"Y:%d",Yaw_Angle);
	  
	 }

	
	
	
	
	//显示MPU初始化异常或读数异常
	if(My_MPUERR_OLED!=0||My_MPUData_OLED!=0)
		{
			
			if(My_MPUERR_OLED!=0&&My_MPUData_OLED==0)
				{
					OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
					OLED_SetBrush (&oled,PEN_COLOR_BLACK);
					OLED_SetCursor (&oled,0,64);
					OLED_DrawString (&oled,"MPU Init ERR");
				}
			else if(My_MPUERR_OLED==0&&My_MPUData_OLED!=0)
				{
					OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
					OLED_SetBrush (&oled,PEN_COLOR_BLACK);
					OLED_SetCursor (&oled,0,64);
					OLED_DrawString (&oled,"MPU DATA ERR");
				
				}
			else
				{
					OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
					OLED_SetBrush (&oled,PEN_COLOR_BLACK);
					OLED_SetCursor (&oled,0,64);
					OLED_DrawString (&oled,"MPU ALL ERR");
				
				}
		
		}
		else //显示MPU的解算角度数据
		{
			OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
			OLED_SetBrush (&oled,PEN_COLOR_BLACK);
			OLED_SetCursor (&oled,0,64);
			OLED_Printf(&oled,"MP:%d",(int)MPU_Pitch);

			OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
			OLED_SetBrush (&oled,PEN_COLOR_BLACK);
			OLED_SetCursor (&oled,64,64);
			OLED_Printf(&oled,"MR:%d",(int)MPU_ROLL);
		}
	
	
	//显示当前模式
	switch (sys_Mode)
		{
			case 1:
					OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
					OLED_SetBrush (&oled,PEN_COLOR_BLACK);
					OLED_SetCursor (&oled,OLED_GetStrWidth (&oled,"MODE:")+3,20);
					OLED_DrawString (&oled,"1");
					break;
			
			case 2:
					OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
					OLED_SetBrush (&oled,PEN_COLOR_BLACK);
					OLED_SetCursor (&oled,OLED_GetStrWidth (&oled,"MODE:")+3,20);
					OLED_DrawString (&oled,"2");
					break;
			default :
					OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
					OLED_SetBrush (&oled,PEN_COLOR_BLACK);
					OLED_SetCursor (&oled,OLED_GetStrWidth (&oled,"MODE:")+3,20);
					OLED_DrawString (&oled,"3");
					break;
		
		}
	
	//显示最近一次按键类型	
	if(My_button_State==1)
		{
			OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
			OLED_SetBrush (&oled,PEN_COLOR_BLACK);
			OLED_SetCursor (&oled,OLED_GetStrWidth (&oled,"BUTTON_EVENT:")+1,32);
			OLED_DrawString (&oled,"short");
		}
	else
		{
			if(My_button_State==2)
				{
					OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
					OLED_SetBrush (&oled,PEN_COLOR_BLACK);
					OLED_SetCursor (&oled,OLED_GetStrWidth (&oled,"BUTTON_EVENT:")+1,32);
					OLED_DrawString (&oled,"long");
				}
			
		}

		
		
	
	//显示命令
	if(strcmp(current_command,"PITCH")==0)
		{
			OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
			OLED_SetBrush (&oled,PEN_COLOR_BLACK);
			OLED_SetCursor (&oled,OLED_GetStrWidth (&oled,"COMMAND:")+1,44);
			OLED_Printf(&oled,"Pitch<%d>",temp_Pitch);
		}
	else if(strcmp(current_command,"YAW")==0)
		{
			OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
			OLED_SetBrush (&oled,PEN_COLOR_BLACK);
			OLED_SetCursor (&oled,OLED_GetStrWidth (&oled,"COMMAND:")+1,44);
			OLED_Printf(&oled,"Yaw<%d>",temp_Yaw);
		}
	else
		{
			OLED_SetPen (&oled,PEN_COLOR_WHITE,1);
			OLED_SetBrush (&oled,PEN_COLOR_BLACK);
			OLED_SetCursor (&oled,OLED_GetStrWidth (&oled,"COMMAND:")+1,44);
			OLED_DrawString (&oled,"None");
		}
			
		
	
		OLED_SendBuffer (&oled);//上传

//		if(OLED_SendBuffer (&oled)!=0)
//			{
//			  sys_checkpoint =1;
//			
//			}//上传
		
		
}


