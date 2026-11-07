#include "stm32f10x.h"
#include "jka_MPU6050.h"
#include "Delay.h"
#include "bsp_oled.h"
#include "bsp_led.h"

uint8_t  My_si2cMPU_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct={0};
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_11|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	return 0;
}



//void My_MPUdelayus(uint32_t us)
//{
//	uint32_t n=us*8;
//	
//	for(uint32_t i=0;i<n;i++);

//}



//
//
//（向SCL写入 高电平-1/低电平-0）
void My_sclMPU_write(uint8_t level)
{

	if(level==0)
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_RESET);
		
		}
	else
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_SET);
		}

		DelayUs(2);

}

//
//
//（向SDA写入 释放/高电平-1 低电平-0）
void My_sdaMPU_write(uint8_t level)
{

	if(level==0)
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);
		
		}
	else
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);
		}

	DelayUs(2);

}

//
//
//SDA读取数据
//（返回：引脚的电平状态）
uint8_t My_sdaMPU_read(void)
{
	uint8_t Bit=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10);
	DelayUs(2);
	
	return Bit;

}

//
//
//发送起始位
void My_si2cMPU_SendStart(void)
{
	
	My_sdaMPU_write(1);//先拉高SDA,准备产生下降沿
	My_sclMPU_write (1);//拉高SCL
	My_sdaMPU_write(0);//在SCL高电平时拉低SDA
	My_sclMPU_write(0);//SCL写0，为传送数据做准备

}
//
//
//发送终止位
void My_sic2MPU_SendStop(void)
{
	My_sclMPU_write(0);//拉低SCL，防止在SCL高位时拉低SDA造成起始位的发送
	My_sdaMPU_write(0);//拉低SDA，为产生上升沿做准备
	
	My_sclMPU_write(1);//拉高SCL
	
	My_sdaMPU_write(1);//释放SDA,发送终止信号

}

//
//
///发送一个字节
//（发送：要发送的数据）
void My_si2cMPU_SendByte(uint8_t Byte)
{

	for(int8_t i=7;i>=0;i--)//将字节以8个比特位的形式发送出去
		{
			
			
			if((Byte&(0x01<<i))!=0)//检测字节第i位是否为1
				{
					My_sdaMPU_write(1);
				
				}
			else
				{
					My_sdaMPU_write(0);
				}

		
			My_sclMPU_write(1);//拉高SCL，发送数据
			My_sclMPU_write(0);//拉低SCL，为传输下一个数据做准备
		
		
		}
		

	
}

//
//
//接收一个字节
//（返回：从机发送的字节）
uint8_t My_si2cMPU_ReByte(void)
{
	
	uint8_t byte=0;
	My_sdaMPU_write(1); //释放SDA,让从机能发送数据
	
	for(int8_t i=7;i>=0;i--)//逐个读取字节的八个比特位
		{
			My_sclMPU_write(1);//拉高SCL,读取数据
			
			if(My_sdaMPU_read()!=0)//判断第i位读到的是否为1
				{
					byte|=(0x01<<i);//是则置1
				}
				
			My_sclMPU_write(0);//拉低SCL， 让从机准备下一个发送的数据/最后一个比特位接收完后为主机发送ACK/NACK做准备
		}
	
		
	return byte;
	

}

//
//主机发送ACK/NACK应答从机
//（发送：ACK-0,NACK-1）
void MPU_sendACK(uint8_t ACKBit)
{
	My_sdaMPU_write(ACKBit);
	My_sclMPU_write(1);//拉高SCL,从机读取
	My_sclMPU_write(0);//拉低SCL，为发送终止位做准备
}

//
//主机读取ACK/NACK
//（返回：ACK-0,NACK-1）
uint8_t MPU_REACK(void)
{
	
	My_sdaMPU_write(1);//释放SDA,使从机能通过拉高/低SDA应答
	My_sclMPU_write(1);//拉高SCL，从机发送ACK/NACK
	uint8_t ACKBit=My_sdaMPU_read();//接收
	My_sclMPU_write(0);//拉低SCL,为下一个数据/终止位的发送做准备
	
	return ACKBit ;
}

//
//初始化MPU6050专用
//（发送：寄存器地址，数据）
//（返回:是否成功）
int My_si2cMPU_Send(uint8_t RegAddr,uint8_t Data)
{

	My_si2cMPU_SendStart();
	
	//寻址
	My_si2cMPU_SendByte(0XD0);//发送从机地址
	if(MPU_REACK()!=0)//判断应答ACK/NACK
		{
			My_sic2MPU_SendStop ();
			My_MPUERR_OLED=1;
			sys_checkpoint=1;
			return -1;
		}
	
	My_si2cMPU_SendByte(RegAddr);//发送寄存器地址
	if(MPU_REACK()!=0)
		{
			My_sic2MPU_SendStop ();
			My_MPUERR_OLED=1;
			sys_checkpoint=1;
			return -2;
		}
	
	My_si2cMPU_SendByte(Data);//发送数据
	if(MPU_REACK()!=0)
		{
			My_sic2MPU_SendStop ();
			My_MPUERR_OLED=1;
			sys_checkpoint=1;
			return -3;
		}
	
	My_sic2MPU_SendStop();
	
	return 0;
}	
//
//读取寄存器里的数据
//（发送：寄存器的地址）
//（返回：寄存器里的数据）
uint8_t  My_si2cMPU_ReReg(uint8_t RegAddr)
{
	
	uint8_t Data;
	
	My_si2cMPU_SendStart();

	//寻址
	My_si2cMPU_SendByte(0XD0);//发送从机地址
	if(MPU_REACK()!=0)//应答NACK
		{
			My_sic2MPU_SendStop ();
			My_MPUData_OLED=1;
			sys_checkpoint=1;
			return -1;//寻址失败
		}
	
	
	
	My_si2cMPU_SendByte(RegAddr);//发送寄存器地址
	if(MPU_REACK()!=0)
		{
			My_sic2MPU_SendStop ();
			My_MPUData_OLED=1;
			sys_checkpoint=1;
			return -1;//寻址失败
		}

	
	My_si2cMPU_SendStart();//重复起始
	My_si2cMPU_SendByte(0xD0|0x01);//发送从机地址，读写位1，表示读取	
	if(MPU_REACK()!=0)
		{
			My_sic2MPU_SendStop ();
			My_MPUData_OLED=1;
			sys_checkpoint=1;
			return -1;//读取数据失败
		}
		
	Data=My_si2cMPU_ReByte ();//接收寄存器的数据
	MPU_sendACK(1);//释放总线，给从机非应答NACK
	
	My_sic2MPU_SendStop ();

	return Data;
		
}

