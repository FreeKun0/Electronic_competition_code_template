#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "stm32f10x.h"

#define MAX_ROW_COUNT 10              // 最大行数
#define MAX_COL_COUNT 10              // 最大列数

// 定义引脚配置结构体
typedef struct
{
	GPIO_TypeDef* GPIOx;    // GPIO端口
	uint16_t Pin;           // 引脚号
} Key_PinConfig_t;

// 定义矩阵键盘配置结构体
typedef struct
{
    uint8_t RowCount;                  // 行数
    uint8_t ColCount;                  // 列数
    Key_PinConfig_t RowPins[MAX_ROW_COUNT];  // 行引脚配置
    Key_PinConfig_t ColPins[MAX_COL_COUNT];  // 列引脚配置
} Key_MatrixConfig_t;

extern const Key_MatrixConfig_t Key_MatrixConfig;

#define	KEYB1_PRES	1
#define	KEYB2_PRES	2
#define	KEYB3_PRES	3
#define	KEYB4_PRES	4
#define	KEYB5_PRES	5
#define	KEYB6_PRES	6
#define	KEYB7_PRES	7
#define	KEYB8_PRES	8
#define	KEYB9_PRES	9
#define	KEYB10_PRES	10
#define	KEYB11_PRES	11
#define	KEYB12_PRES	12
#define	KEYB13_PRES	13
#define	KEYB14_PRES	14
#define	KEYB15_PRES	15
#define	KEYB16_PRES	16

/*
1	2	3	A
4	5	6	B
7	8	9	C
0	.	E	D

*/
#define	NUM1_PRES	KEYB1_PRES
#define	NUM2_PRES	KEYB2_PRES
#define	NUM3_PRES	KEYB3_PRES
#define	NUM4_PRES	KEYB5_PRES
#define	NUM5_PRES	KEYB6_PRES
#define	NUM6_PRES	KEYB7_PRES
#define	NUM7_PRES	KEYB9_PRES
#define	NUM8_PRES	KEYB10_PRES
#define	NUM9_PRES	KEYB11_PRES
#define	NUM0_PRES	KEYB13_PRES
#define	NUMP_PRES	KEYB14_PRES

void Keyboard_Init(void);
uint8_t Keyboard_GetNum(void);
void Keyboard_Tick(void);
uint8_t Keyboard_Getstate(void);

#endif
