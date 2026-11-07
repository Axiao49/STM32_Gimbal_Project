#ifndef _BSP_USART_H_
#define _BSP_USART_H_

#include "stm32f10x.h"
#include <stdio.h>

void My_USART_Init(void);
void My_USART_SendBytes(USART_TypeDef *USARTx,uint8_t *pData,uint16_t Size);
void My_USART_SendByte(USART_TypeDef *USARTx,uint8_t Data);
int fputc(int ch,FILE *f);
void My_USART_Word(void);
void My_USART_Mode(void);
extern char current_command[33];
extern 	int32_t temp_Pitch;
extern  int32_t temp_Yaw;		




#endif


