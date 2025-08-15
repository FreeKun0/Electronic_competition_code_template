#include "key.h"

uint8_t Key_Num;

void Key_Init(void)
{
	KEY1_GPIO_CLK_ENABLE();
	KEY2_GPIO_CLK_ENABLE();
	KEY3_GPIO_CLK_ENABLE();
	KEY4_GPIO_CLK_ENABLE();
//	KEY5_GPIO_CLK_ENABLE();
//	KEY6_GPIO_CLK_ENABLE();
//	
//	KEY11_GPIO_CLK_ENABLE();
//	KEY12_GPIO_CLK_ENABLE();
//	KEY13_GPIO_CLK_ENABLE();
//	KEY14_GPIO_CLK_ENABLE();
//	
//	KEY_RESET_GPIO_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_Pin;
	GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_Pin;
	GPIO_Init(KEY2_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_Pin;
	GPIO_Init(KEY3_GPIO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_Pin;
	GPIO_Init(KEY4_GPIO_Port, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = KEY5_GPIO_Pin;
//	GPIO_Init(KEY5_GPIO_Port, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = KEY6_GPIO_Pin;
//	GPIO_Init(KEY6_GPIO_Port, &GPIO_InitStructure);	
//	
//	
//	GPIO_InitStructure.GPIO_Pin = KEY11_GPIO_Pin;
//	GPIO_Init(KEY11_GPIO_Port, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = KEY12_GPIO_Pin;
//	GPIO_Init(KEY12_GPIO_Port, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = KEY13_GPIO_Pin;
//	GPIO_Init(KEY13_GPIO_Port, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = KEY14_GPIO_Pin;
//	GPIO_Init(KEY14_GPIO_Port, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = KEY_RESET_GPIO_Pin;
//	GPIO_Init(KEY_RESET_GPIO_Port, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if (Key_Num)
	{
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
	return 0;
}



uint8_t Key_GetState(void)
{
	if (GPIO_ReadInputDataBit(KEY1_GPIO_Port, KEY1_GPIO_Pin) == 0)
	{
		return KEY1_PRES;
	}
	if (GPIO_ReadInputDataBit(KEY2_GPIO_Port, KEY2_GPIO_Pin) == 0)
	{
		return KEY2_PRES;
	}
	if (GPIO_ReadInputDataBit(KEY3_GPIO_Port, KEY3_GPIO_Pin) == 0)
	{
		return KEY3_PRES;
	}
	if (GPIO_ReadInputDataBit(KEY4_GPIO_Port, KEY4_GPIO_Pin) == 0)
	{
		return KEY4_PRES;
	}
//	if (GPIO_ReadInputDataBit(KEY5_GPIO_Port, KEY5_GPIO_Pin) == 0)
//	{
//		return KEY5_PRES;
//	}
//	if (GPIO_ReadInputDataBit(KEY6_GPIO_Port, KEY6_GPIO_Pin) == 0)
//	{
//		return KEY6_PRES;
//	}
//	
//	if (GPIO_ReadInputDataBit(KEY11_GPIO_Port, KEY11_GPIO_Pin) == 0)
//	{
//		return KEY11_PRES;
//	}
//	if (GPIO_ReadInputDataBit(KEY12_GPIO_Port, KEY12_GPIO_Pin) == 0)
//	{
//		return KEY12_PRES;
//	}
//	if (GPIO_ReadInputDataBit(KEY13_GPIO_Port, KEY13_GPIO_Pin) == 0)
//	{
//		return KEY13_PRES;
//	}
//	if (GPIO_ReadInputDataBit(KEY14_GPIO_Port, KEY14_GPIO_Pin) == 0)
//	{
//		return KEY14_PRES;
//	}
//	if (GPIO_ReadInputDataBit(KEY_RESET_GPIO_Port, KEY_RESET_GPIO_Pin) == 0)
//	{
//		return KEY_RESET_PRES;
//	}
	
	return 0;
}

void Key_Tick(void)
{
	static uint8_t Count;
	static uint8_t CurrState, PrevState;
	
	Count ++;
	if (Count >= 20)
	{
		Count = 0;
		
		PrevState = CurrState;
		CurrState = Key_GetState();
		
		if (CurrState == 0 && PrevState != 0)
		{
			Key_Num = PrevState;
		}
	}
}
