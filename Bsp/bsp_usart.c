#include "main.h"
#include <stdio.h>
#include <string.h>

#include "bsp_button.h"
#include "bsp_servo.h"
#include "bsp_mpu6050.h"
#include "bsp_led.h"

volatile uint8_t My_USART_Buffer[33]={0};//缓冲区接收数据+ /0
volatile uint8_t My_Buffer_Size=0;//记录读取的数据数量
volatile uint8_t My_USART_DataREC=0;//标志位，判断是否接收完成
char current_command[33]={0};//定义数组，准备接收指令		char command[100]={0};
uint8_t DataOR_Check=0;
int32_t temp_Pitch=90;//显示舵机运动角度
int32_t temp_Yaw=90;		

uint8_t rx_data=0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	
	if(huart->Instance==USART1)//判断是否可读取数据
		{
			uint8_t ch=rx_data;//
			
			
			if(ch!='\n'&&ch!='\r')//判断是否为结束标志
				{
					if(My_Buffer_Size<32)//判断记录到的字符数量
						{
							My_USART_Buffer[My_Buffer_Size]=ch;//接收数据
							My_Buffer_Size++;//更新记录的数量
							
						}
					else 
						{
							DataOR_Check=1;
						}
				}
			else	
				{
					if(My_Buffer_Size>0)//补：判断是否为空指令，如是则舍
						{
							My_USART_Buffer[My_Buffer_Size]='\0';//加上终止位
							My_USART_DataREC=1;//标志位置1
							My_Buffer_Size=0;//记录数清零，为下一次记录做准备
						}
					
				}
				HAL_UART_Receive_IT(&huart1, &rx_data, 1);
		}	

}


//USART发送多个数据（串口类型，发送的数据，数据大小）
void My_USART_SendBytes(USART_TypeDef *USARTx,uint8_t *pData,uint16_t Size)
{

	if(USARTx==USART1)
	{
		HAL_UART_Transmit(&huart1,pData,Size,HAL_MAX_DELAY);

	}

}


//USART发送单个数据（发送的串口类型，发送的数据）
void My_USART1_SendByte(USART_TypeDef *USARTx,uint8_t Data)
{
	My_USART_SendBytes(USARTx,&Data,1);

}



//重定向printf ->可发送字符（发送的字符）
int _write(int file,char *ptr,int len)
{

    HAL_UART_Transmit(&huart1,(uint8_t*)ptr,len,HAL_MAX_DELAY);	
	return len;


}






//判断数据内容并做出应答
void My_USART_Word(void)
{

	if(My_USART_DataREC==0)//判断是否接收数据
		{
		  		
			return ;
		}
		
		
	My_LED3_Check ();//收到完整一帧指令，翻转LED3
	if(DataOR_Check==1)
		{
			printf ("ERR\r\n");
			My_USART_DataREC=0;//标志位置0
			DataOR_Check=0;
			return;
		}
		
		
	

	
		for(int i=0;My_USART_Buffer[i]!='\0';i++)//将小写全部转为大写
		{
			if(My_USART_Buffer[i]>='a'&&My_USART_Buffer[i]<='z')
				{
					My_USART_Buffer[i]-=32;
				
				}
				
				
		}
	
		
		
//	char current_command[33]={0};
	int32_t  angle=0;//记录传入指令的角度，为舵机转向运动做准备
	int JudgePoint=0;
	int sscanf_size=0;
	JudgePoint=sscanf((char *)My_USART_Buffer,"%s %d%n",current_command ,&angle,&sscanf_size);//接收数据,并为判断是否为舵机指令做准备	

	

		//比较字符串内容是否一样
	if(strcmp((const char *)My_USART_Buffer,"AT")==0)//(My_USART_Buffer[0]=='A'&&My_USART_Buffer[1]=='T')
		{
			printf("OK\r\n");
		
		}
		else if(strcmp((const char*)current_command,"STATUS")==0)
			{
				printf("OK MODE=%d\r\n",sys_Mode);

			}
		else if(strcmp((const char* )current_command,"YAW")==0||strcmp((const char*)current_command,"PITCH")==0)
		//比较字符，判断指令		
				{
					
						
						if(sys_Mode!=1)//判断是否是模式1
							{	
								printf ("ERR\r\n");
							} 
						else if(JudgePoint!=2||sscanf_size!=strlen ((char *)My_USART_Buffer))//判断指令是否满足基本格式要求（英文+数字），以及结尾是否有多余符号
							{
								printf ("ERR\r\n");
							}	
						else if(angle<0||angle>180)//判断指令角度是否符合要求
							{
								printf ("ERR\r\n");
							}
						else
							{
								printf ("OK\r\n");
								
				
								if(strcmp(current_command,"YAW")==0)//判断，记录角度
									{
										temp_Yaw =angle;
										Yaw_Angle =180-angle;//记录角度
									}
								else
									{
										temp_Pitch =angle;
										Pitch_Angle =180-angle;
									}
							}
				
					}
		else if(strcmp((const char*)current_command,"CAL")==0)//姿态板执行零位校准判断
			{
				
				
				Calibration_point=1;//标志位置1
				
			}
		else
			{
				printf ("ERR\r\n");
			}
	
	
	
	My_USART_DataREC=0;//标志位置0		

}


//事件回传
void My_USART_Mode(void)
{

	if(My_button_duration>=50&&My_button_duration<1000)//判断时间长短
		{
			printf("KEY S\r\n");
			My_button_duration=0;//时间记录置零

		}
	else
		{
			if(My_button_duration>1000)
				{
					printf("KEY L\r\n");
					My_button_duration=0;
				}
			
		}

	

if(sys_Mode!=sys_LastMode)//模式改变后回传改变后的模式
		{
			printf("MODE <%d>\r\n",sys_Mode);
	
			
			sys_LastMode=sys_Mode;
			
		}
	




}



