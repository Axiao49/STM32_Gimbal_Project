#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"
#include "oled.h"


#include "bsp_led.h"
#include "bsp_timer.h"
#include "bsp_button.h"
#include "bsp_usart.h"
#include "bsp_si2c.h"
#include "bsp_oled.h"
#include "bsp_servo.h"
#include "bsp_si2cMPU.h"
#include "bsp_mpu6050.h"
#include "bsp_lightSensor.h"

volatile uint32_t My_oled_StartTime=0;
volatile uint32_t My_mpu_StartTime=0;
volatile uint32_t My_lightSensor_StartTime=0;
volatile uint32_t My_servo_Time=0;


int main(void)
{
	My_LED_Init();
	My_Check_LED ();
	My_Timer_Init();
	My_button_Init();
	My_USART_Init();
	
	
	uint8_t  resultoled1=My_si2c_Init();
	uint8_t resultmpu1=My_si2cMPU_Init();
	
	
	uint8_t  resultoled2=My_oled_Init();
	My_servo_Init();
	uint8_t resultmpu2=My_MPU6050_Init();
	
	LightSensor_Init();
	
	
		if(resultoled1!=0||resultoled2!=0)
			{
				sys_checkpoint=1;
			}
			
		

	Delay(500);
	if(MPU6050_AngleCalibration()!=0)
		{
			printf ("AUTO CAL ERR\r\n");
		}
	else 
		{
			printf ("AUTO CAL OK\r\n");
		}
	
	
	
	while(1)
	{
		My_Check2_LED1 ();
		My_button_Press();
		My_USART_Mode();
		My_Mode_CheckLED2();
		My_USART_Word();
		
		
		if(currentTick-My_oled_StartTime>=200)
			{
				My_oled_printf();
				My_oled_StartTime=currentTick ;
			}
		
		if(Calibration_point==1)
			{
				Calibration_point=0;
				
				if(MPU6050_AngleCalibration()==0)
					{
						printf ("OK\r\n");
					}
				else
					{
						printf ("ERR CAL\r\n");
						
					}
			}
			
			
		if(currentTick-My_mpu_StartTime>=20)
			{
				MPU6050_UpdateAngle();//
				My_mpu_StartTime=currentTick ;
			}
			
			
			
		if(currentTick-My_servo_Time>=20)	
			{
				Servo_Move();
				My_servo_Time=currentTick;
			}
	}
}


