#include "laser.h"


void Laser_P_Init(void)
{
	Laser_Purple_GPIO_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = Laser_Purple_GPIO_Pin;
	GPIO_Init(Laser_Purple_GPIO_Port, &GPIO_InitStructure);
	
	
//	GPIO_SetBits(Laser_Purple_GPIO_Port, Laser_Purple_GPIO_Pin);
	GPIO_ResetBits(Laser_Purple_GPIO_Port, Laser_Purple_GPIO_Pin);
}

void Laser_P_OFF(void)
{
	GPIO_ResetBits(Laser_Purple_GPIO_Port, Laser_Purple_GPIO_Pin);
}

void Laser_P_ON(void)
{
	GPIO_SetBits(Laser_Purple_GPIO_Port, Laser_Purple_GPIO_Pin);
}

void Laser_P_Turn(void)
{
	if (GPIO_ReadOutputDataBit(Laser_Purple_GPIO_Port, Laser_Purple_GPIO_Pin) == 0)
	{
		GPIO_SetBits(Laser_Purple_GPIO_Port, Laser_Purple_GPIO_Pin);
	}
	else
	{
		GPIO_ResetBits(Laser_Purple_GPIO_Port, Laser_Purple_GPIO_Pin);
	}
}
//void Beep_ms(uint32_t xms)
//{
//	Beep_ON();
//	Delay_ms(xms);
//	Beep_OFF();
//}



