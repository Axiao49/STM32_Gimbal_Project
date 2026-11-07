#include "stm32f10x.h"
#include "delay.h"


uint8_t  My_si2c_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct={0};
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	return 0;
}

//
//
//（向SCL写入 高电平-1/低电平-0）
void My_scl_write(uint8_t level)
{

	if(level==0)
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_RESET);//SCL写低电平
		
		}
	else
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_8,Bit_SET);//SCL写高电平
		}



}

//
//
//（向SDA写入 释放/高电平-1 低电平-0）
void My_sda_write(uint8_t level)
{

	if(level==0)
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_RESET);//SDA写低电平
		
		}
	else
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_SET);//SDA写高电平
		}



}

//
//SDA读取数据
//（返回；引脚的电平状态）
uint8_t My_sda_read(void)
{

	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==Bit_SET )//读取输入引脚的电平
		{
			return 1;
		}
	else
		{
			return 0;
		}

}






//
//
//发送起始位
void My_si2c_SendStart(void)
{
	
	My_sda_write(1);//先拉高SDA，准备产生下降沿发起始位
	My_scl_write (1);//拉高SCL
	DelayUs (1);
	My_sda_write(0);//在SCL高电平时拉低SDA
	DelayUs(1);

}

//
//
//发送终止位
void My_sic2_SendStop(void)
{
	My_scl_write(0);//先拉低SCL,防止SDA拉低时被作为发送起始信号
	My_sda_write(0);//拉低SDA，为产生上升沿做准备
	DelayUs(1);
	
	My_scl_write(1);//拉高SCL
	DelayUs(1);
	
	My_sda_write(1);//拉高/释放SDA,发送终止信号
	DelayUs(1);


}

//
//发送一个字节
//（发送：要发送的字节）
uint8_t My_si2c_SendByte(uint8_t Byte)
{

	for(int8_t i=7;i>=0;i--)//将字节转为八个比特位逐个传入
		{
			My_scl_write(0);//将SCL拉低，准备发送的数据
			
			if((Byte&(0x01<<i))!=0)//判断第i位是否为1，并写入
				{
					My_sda_write(1);
				
				}
			else
				{
					My_sda_write(0);
				}

			DelayUs(1);
			My_scl_write(1);//拉高SCL，传输数据
			DelayUs(1);
		
		
		}
		
		//读取ACK或NAK
		My_scl_write(0);//拉低SCL
		My_sda_write(1);//释放SDA，等待从机回应ACK或NAK
		DelayUs(1);
		
		My_scl_write(1);//拉高SCL,准备读取
		DelayUs(1);
		
		return My_sda_read();//读取并返回，0则从机拉低SDA,回应ACK,1则从机无应答，即NACK

	
}


//
//接收一个字节
//（发送：ACK-1/NACK-0）
//（返回：接收的字节）
uint8_t My_si2c_ReByte(uint8_t ACK)
{
	
	uint8_t byte=0;
	
	for(int8_t i=7;i>=0;i--)//逐个读取字节的八个比特位
		{
			My_scl_write(0);//拉低SCL,让从机准备发送数据
			My_sda_write (1);//释放SDA,让从机能发送数据
			DelayUs(1);
			
			My_scl_write(1);//拉高SCL，读取数据
			DelayUs(1);
			
			
			if(My_sda_read()!=0)//判断读取的第i位是否为1
				{
					byte|=(0x01<<i);//是则置1
				}
				
			
		}
	
	//回复ACK或NACK	
	My_scl_write (0);//拉低SCL,	准备改变SDA
	My_sda_write(!ACK);//函数传入值为1（ACK）时,取反为0，拉低SDA即发送ACK.反之则释放SDA即发送NACK
	DelayUs(1);
	My_scl_write(1);//拉高SCL,从机读取ACK/NACK
	DelayUs(1);
	

	return byte;//返回读取的字节
	

}

//
//发送多个数据
//（发送：从机七位地址+读写位，传输的数据，数据大小）
//（返回：发送结果）
int My_si2c_SendBytes(uint8_t Addr,uint8_t *pData,uint16_t Size)
{

	My_si2c_SendStart();
	
	if(My_si2c_SendByte(Addr&0xfe)!=0)//1111 1110 最低位清0 传进的Adrr必须前七位为地址位，即原始七位地址已左移并加入读写位！！！！！
		{
			My_sic2_SendStop();
			return -1;//寻址失败
		
		}
	
		
	for(uint32_t i=0;i<Size;i++)//逐个发送数据
		{
			if(My_si2c_SendByte (pData[i])!=0)
				{
					My_sic2_SendStop();
					return -2;//发送失败
				}
		
		}

	My_sic2_SendStop();
	
	return 0;//发送成功
}	




//
//读取多个数据
//（发送：从机七位地址+读写位，读取数据缓冲区，数据大小）
int My_si2c_ReBytes(uint8_t Addr,uint8_t *pBuffer,uint16_t Size)
{

	My_si2c_SendStart();

	if(My_si2c_SendByte(Addr&0x01)!=0)
		{
			My_sic2_SendStop();
			return -1;//寻址失败
		
		}
	
	
	for(uint32_t i=0;i<Size-1;i++)//逐个读取数据至Size-1个，发送ACK	
		{
			pBuffer[i]=My_si2c_ReByte(1);
		
		}
		
		pBuffer [Size-1]=My_si2c_ReByte (0);//读取最后一个数据，发送NACK

		My_sic2_SendStop();
		
		return 0;//读取成功
}


