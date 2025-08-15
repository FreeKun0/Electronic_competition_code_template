#ifndef __UART4_H
#define __UART4_H

#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
//#include "parameter.h"
//#include "control.h"
//#include "LED.h"


#define UART4_RX_SIZE 10

extern uint16_t UART4_RxBuffer[UART4_RX_SIZE];
extern uint8_t UART4_RxErr;


void UART4_Init(uint32_t BaudRate);
void UART4_SendData(void);

/* 串口默认设置 */
extern uint8_t UART4_RxData;		//定义串口接收的数据变量
extern uint8_t UART4_RxFlag;		//定义串口接收的标志位变量

void UART4_SendByte(uint8_t Byte);
void UART4_SendArray(uint8_t *Array, uint16_t Length);
void UART4_SendString(char *String);
void UART4_SendNumber(uint32_t Number, uint8_t Length);
void UART4_Printf(char *format, ...);

uint8_t UART4_GetRxFlag(void);
uint8_t UART4_GetRxData(void);

#endif
