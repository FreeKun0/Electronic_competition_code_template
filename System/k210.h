#ifndef __K210_H
#define __K210_H

#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "parameter.h"
#include "control.h"
#include "LED.h"
#include "YT.h"


#define USART3_RX_SIZE 15

extern uint16_t USART3_RxBuffer[USART3_RX_SIZE];
extern uint8_t USART3_RxErr;

void USART3_Init(uint32_t BaudRate);
void USART3_SendData(void);

/* 串口默认设置 */
extern uint8_t USART3_RxData;		//定义串口接收的数据变量
extern uint8_t USART3_RxFlag;		//定义串口接收的标志位变量

void USART3_SendByte(uint8_t Byte);
void K210_SendByte(uint8_t Byte);
void USART3_SendArray(uint8_t *Array, uint16_t Length);
void USART3_SendString(char *String);
void USART3_SendNumber(uint32_t Number, uint8_t Length);
void USART3_Printf(char *format, ...);

uint8_t USART3_GetRxFlag(void);
uint8_t USART3_GetRxData(void);

#endif
