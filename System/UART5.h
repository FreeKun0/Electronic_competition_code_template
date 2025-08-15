#ifndef __UART5_H
#define __UART5_H

#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "parameter.h"
#include "control.h"
//#include "LED.h"


#define UART5_RX_SIZE 50

extern uint16_t UART5_RxBuffer[UART5_RX_SIZE];
extern uint8_t UART5_RxErr;

void UART5_Init(uint32_t BaudRate);
//void UART5_SendData(void);

/* 串口默认设置 */
extern uint8_t UART5_RxData;		//定义串口接收的数据变量
extern uint8_t UART5_RxFlag;		//定义串口接收的标志位变量

void UART5_SendByte(uint8_t Byte);
void UART5_SendArray(uint8_t *Array, uint16_t Length);
void UART5_SendString(char *String);
void UART5_SendNumber(uint32_t Number, uint8_t Length);
void UART5_Printf(char *format, ...);

uint8_t UART5_GetRxFlag(void);
uint8_t UART5_GetRxData(void);


extern float test_float;
extern int test_int;


void HMI_Printf(char *format, ...);
extern uint8_t HMI_RxFlag;
extern uint8_t HMI_packet_length;
uint8_t HMI_GetRxFlag(void);
void HMI_Handling(void);

void HMI_SendString(char* name, char* showdata);
void HMI_SendNum(char* name, int num);
void HMI_SendFloat(char* name, float num);

#endif
