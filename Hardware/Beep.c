#include "Beep.h"


void Beep_Init(void)
{
	Beep_GPIO_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = Beep_GPIO_Pin;
	GPIO_Init(Beep_GPIO_Port, &GPIO_InitStructure);
	
	
	GPIO_SetBits(Beep_GPIO_Port, Beep_GPIO_Pin);
}

void Beep_ON(void)
{
	GPIO_ResetBits(Beep_GPIO_Port, Beep_GPIO_Pin);
}

void Beep_OFF(void)
{
	GPIO_SetBits(Beep_GPIO_Port, Beep_GPIO_Pin);
}

void Beep_Turn(void)
{
	if (GPIO_ReadOutputDataBit(Beep_GPIO_Port, Beep_GPIO_Pin) == 0)
	{
		GPIO_SetBits(Beep_GPIO_Port, Beep_GPIO_Pin);
	}
	else
	{
		GPIO_ResetBits(Beep_GPIO_Port, Beep_GPIO_Pin);
	}
}
void Beep_ms(uint32_t xms)
{
	Beep_ON();
	Delay_ms(xms);
	Beep_OFF();
}



