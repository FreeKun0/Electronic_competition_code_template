#ifndef __USART2_H
#define __USART2_H

#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
//#include "parameter.h"
//#include "control.h"
//#include "LED.h"


#define USART2_RX_SIZE 15

extern uint16_t USART2_RxBuffer[USART2_RX_SIZE];
extern uint8_t USART2_RxErr;

void USART2_Init(uint32_t BaudRate);
void USART2_SendData(void);

/* ����Ĭ������ */
extern uint8_t USART2_RxData;		//���崮�ڽ��յ����ݱ���
extern uint8_t USART2_RxFlag;		//���崮�ڽ��յı�־λ����

void USART2_SendByte(uint8_t Byte);
void USART2_SendArray(uint8_t *Array, uint16_t Length);
void USART2_SendString(char *String);
void USART2_SendNumber(uint32_t Number, uint8_t Length);
void USART2_Printf(char *format, ...);

uint8_t USART2_GetRxFlag(void);
uint8_t USART2_GetRxData(void);

#endif
