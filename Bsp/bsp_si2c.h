#ifndef _BSP_SI2C_H_
#define _BSP_SI2C_H_

#include "stm32f10x.h"

uint8_t  My_si2c_Init(void);
void My_scl_write(uint8_t level);
void My_sda_write(uint8_t level);
uint8_t My_sda_read(void);
void My_delayus(uint32_t us);
void My_si2c_SendStart(void);
void My_sic2_SendStop(void);
uint8_t My_si2c_SendByte(uint8_t Byte);//发送一个字节
uint8_t My_si2c_ReByte(uint8_t ACK);//接收一个字节
int My_si2c_SendBytes(uint8_t Addr,uint8_t *pData,uint16_t Size);//发送多个字节
int My_si2c_ReBytes(uint8_t Addr,uint8_t *pBuffer,uint16_t Size);//接收多个字节

	
#endif
