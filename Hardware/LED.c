#include "led.h"

void LED_Init(void)
{
#if defined(LED_RED)
	Red_GPIO_CLK_ENABLE();
#endif
#if defined(LED_GREEN)
	Green_GPIO_CLK_ENABLE();
#endif
#if defined(LED_YELLOW)
	Yellow_GPIO_CLK_ENABLE();
#endif
#if defined(LED_BLUE)
	Blue_GPIO_CLK_ENABLE();
#endif
#if defined(LED_WHITE)
	White_GPIO_CLK_ENABLE();
#endif
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
#if defined(LED_RED)
	GPIO_InitStructure.GPIO_Pin = Red_GPIO_Pin;
	GPIO_Init(Red_GPIO_Port, &GPIO_InitStructure);
#endif
#if defined(LED_GREEN)
	GPIO_InitStructure.GPIO_Pin = Green_GPIO_Pin;
	GPIO_Init(Green_GPIO_Port, &GPIO_InitStructure);
#endif
#if defined(LED_YELLOW)
	GPIO_InitStructure.GPIO_Pin = Yellow_GPIO_Pin;
	GPIO_Init(Yellow_GPIO_Port, &GPIO_InitStructure);
#endif
#if defined(LED_BLUE)
	GPIO_InitStructure.GPIO_Pin = Blue_GPIO_Pin;
	GPIO_Init(Blue_GPIO_Port, &GPIO_InitStructure);
#endif
#if defined(LED_WHITE)
	GPIO_InitStructure.GPIO_Pin = White_GPIO_Pin;
	GPIO_Init(White_GPIO_Port, &GPIO_InitStructure);
#endif
	
	
#if defined(LED_RED)
	GPIO_SetBits(Red_GPIO_Port, Red_GPIO_Pin);
#endif
#if defined(LED_GREEN)
	GPIO_SetBits(Green_GPIO_Port, Green_GPIO_Pin);
#endif
#if defined(LED_YELLOW)
	GPIO_SetBits(Yellow_GPIO_Port, Yellow_GPIO_Pin);
#endif
#if defined(LED_BLUE)
	GPIO_SetBits(Blue_GPIO_Port, Blue_GPIO_Pin);
#endif
#if defined(LED_WHITE)
	GPIO_SetBits(White_GPIO_Port, White_GPIO_Pin);
#endif

//	GPIO_ResetBits(Red_GPIO_Port, Red_GPIO_Pin);
//	GPIO_ResetBits(Green_GPIO_Port, Green_GPIO_Pin);
//	GPIO_ResetBits(Yellow_GPIO_Port, Yellow_GPIO_Pin);
//	GPIO_ResetBits(Blue_GPIO_Port, Blue_GPIO_Pin);
//	GPIO_ResetBits(White_GPIO_Port, White_GPIO_Pin);
}


/* LED_Red */
#if defined(LED_RED)
void Red_ON(void)
{
	GPIO_ResetBits(Red_GPIO_Port, Red_GPIO_Pin);
}

void Red_OFF(void)
{
	GPIO_SetBits(Red_GPIO_Port, Red_GPIO_Pin);
}

void Red_Turn(void)
{
	if (GPIO_ReadOutputDataBit(Red_GPIO_Port, Red_GPIO_Pin) == 0)
	{
		GPIO_SetBits(Red_GPIO_Port, Red_GPIO_Pin);
	}
	else
	{
		GPIO_ResetBits(Red_GPIO_Port, Red_GPIO_Pin);
	}
}
#endif

/* LED_Green */
#if defined(LED_GREEN)
void Green_ON(void)
{
	GPIO_ResetBits(Green_GPIO_Port, Green_GPIO_Pin);
}

void Green_OFF(void)
{
	GPIO_SetBits(Green_GPIO_Port, Green_GPIO_Pin);
}

void Green_Turn(void)
{
	if (GPIO_ReadOutputDataBit(Green_GPIO_Port, Green_GPIO_Pin) == 0)
	{
		GPIO_SetBits(Green_GPIO_Port, Green_GPIO_Pin);
	}
	else
	{
		GPIO_ResetBits(Green_GPIO_Port, Green_GPIO_Pin);
	}
}
#endif



/* LED_Yellow */
#if defined(LED_YELLOW)
void Yellow_ON(void)
{
	GPIO_ResetBits(Yellow_GPIO_Port, Yellow_GPIO_Pin);
}

void Yellow_OFF(void)
{
	GPIO_SetBits(Yellow_GPIO_Port, Yellow_GPIO_Pin);
}

void Yellow_Turn(void)
{
	if (GPIO_ReadOutputDataBit(Yellow_GPIO_Port, Yellow_GPIO_Pin) == 0)
	{
		GPIO_SetBits(Yellow_GPIO_Port, Yellow_GPIO_Pin);
	}
	else
	{
		GPIO_ResetBits(Yellow_GPIO_Port, Yellow_GPIO_Pin);
	}
}
#endif

/* LED_Blue */
#if defined(LED_BLUE)
void Blue_ON(void)
{
	GPIO_ResetBits(Blue_GPIO_Port, Blue_GPIO_Pin);
}

void Blue_OFF(void)
{
	GPIO_SetBits(Blue_GPIO_Port, Blue_GPIO_Pin);
}

void Blue_Turn(void)
{
	if (GPIO_ReadOutputDataBit(Blue_GPIO_Port, Blue_GPIO_Pin) == 0)
	{
		GPIO_SetBits(Blue_GPIO_Port, Blue_GPIO_Pin);
	}
	else
	{
		GPIO_ResetBits(Blue_GPIO_Port, Blue_GPIO_Pin);
	}
}
#endif

/* LED_White */
#if defined(LED_WHITE)
void White_ON(void)
{
	GPIO_ResetBits(White_GPIO_Port, White_GPIO_Pin);
}

void White_OFF(void)
{
	GPIO_SetBits(White_GPIO_Port, White_GPIO_Pin);
}

void White_Turn(void)
{
	if (GPIO_ReadOutputDataBit(White_GPIO_Port, White_GPIO_Pin) == 0)
	{
		GPIO_SetBits(White_GPIO_Port, White_GPIO_Pin);
	}
	else
	{
		GPIO_ResetBits(White_GPIO_Port, White_GPIO_Pin);
	}
}
#endif
