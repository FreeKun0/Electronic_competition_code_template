#include "UART5.h"

uint8_t UART5_RxData;		//定义串口接收的数据变量
uint8_t UART5_RxFlag;		//定义串口接收的标志位变量
uint8_t HMI_RxFlag;		//定义串口接收的标志位变量
uint8_t HMI_packet_length;


char str[100];
char temp_str_to_int[100];
int temp_int;
char change_flag;
int temp_index;
uint8_t len1; // 接收到的数据长度



float test_float;
int test_int;



void HMI_Handling(void)
{
	if(HMI_GetRxFlag())
	{
		if(param.HMI_rx[0] == 0x01)
		{
			strncpy(str, param.HMI_rx + 1, HMI_packet_length - 1);
			for (int i = 0; i < HMI_packet_length - 1; i++)
			{
				if (str[i] == ',')
				{
					temp_index = i;
					break;
				}
			}
			change_flag = str[0];
			strncpy(temp_str_to_int, str + 1, temp_index - 1);
			temp_int = atoi(temp_str_to_int);
			switch (change_flag)
			{
				case 'a':
					test_float = (float)temp_int/100.0f;
					break;
				case 'b':
					test_int = temp_int;
					break;
			}
			for(uint8_t i = 0; i < 100; i++)
			{
				temp_str_to_int[i] = 0;
			}
		}
		else if(param.HMI_rx[0] == 0x02)
		{
			last_TASK = TASK;
			TASK = param.HMI_rx[1];
		}
		else if(param.HMI_rx[0] == 0x03)
		{
			if(param.HMI_rx[1] == 0x01)
			{
				Red_ON();
			}
			if(param.HMI_rx[1] == 0x00)
			{
				Red_OFF();
			}
		}
		
		
		
		else if(param.HMI_rx[0] == 0xFF)
		{
			__set_FAULTMASK(1); //关闭所有中断
			NVIC_SystemReset(); //复位
		}
		
	}
}

void HMI_SendString(char* name, char* showdata)
{
	HMI_Printf("%s=\"%s\"", name, showdata);
}
void HMI_SendNum(char* name, int num)
{
	HMI_Printf("%s=%d", name, num);
}
void HMI_SendFloat(char* name, float num)
{
	HMI_Printf("%s=%d", name, (int)(num * 100));
}

void HMI_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	UART5_SendString(String);		//串口发送字符数组（字符串）
	UART5_SendString("\xff\xff\xff");
}

uint8_t HMI_GetRxFlag(void)
{
	if (HMI_RxFlag == 1)			//如果标志位为1
	{
		HMI_RxFlag = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}

/* 发送 */
//uint8_t UART5SendBuf[5] = {0x52, 0x03, 0x00, 0x00, 0x00};
//void UART5_SendData(void)
//{
//	UART5SendBuf[0] = 0x52;	//帧头0x52
//	UART5SendBuf[1] = 0x03;	/* 记得改长度帧 */
//	
//	UART5SendBuf[2] = 0x22;
//	UART5SendBuf[3] = 0x33;
//	UART5SendBuf[4] = 0x44;

//	UART5_SendArray(UART5SendBuf, 5);
//}

/* 接收 */
uint16_t UART5_RxBuffer[UART5_RX_SIZE] = {0};
uint8_t UART5_RxErr = 0;

void UART5_Receive_Data(uint8_t com_data)
{
	static uint8_t RxCounter = 0;
	static uint8_t RxState = 0;
	static uint8_t packet_length = 0;  // 存储当前数据包长度	

	switch(RxState)
	{
		case 0:  // 等待帧头0x52
		if(com_data == 0x52)
		{
			RxState = 1;
			RxCounter = 0;
			UART5_RxBuffer[RxCounter++] = com_data;  // 存储帧头
		}
		break;

		case 1:  // 接收长度字段
			packet_length = com_data + 2;  // 数据长度 + 帧头(1) + 长度(1)
			if(packet_length > UART5_RX_SIZE)
			{
				RxState = 0;
				UART5_RxErr = 1;  // 包长度超过缓冲区
				for(uint8_t i = 0; i < UART5_RX_SIZE; i++)
				{
					UART5_RxBuffer[i] = 0;
				}
				break;
			}
			RxState = 2;
			UART5_RxBuffer[RxCounter++] = com_data;	//存储长度字段
		break;

		case 2:  // 接收数据
			UART5_RxBuffer[RxCounter++] = com_data;
			if(RxCounter >= packet_length)
			{
				HMI_packet_length = UART5_RxBuffer[1];
				
				for(uint8_t i = 0; i < HMI_packet_length; i++)
				{
					param.HMI_rx[i] = UART5_RxBuffer[i + 2];
				}
//				param.HMI_rx[0] = UART5_RxBuffer[2];
//				param.HMI_rx[1] = UART5_RxBuffer[3];
//				param.HMI_rx[2] = UART5_RxBuffer[4];
//				param.HMI_rx[3] = UART5_RxBuffer[5];
//				param.HMI_rx[4] = UART5_RxBuffer[6];
//				param.HMI_rx[5] = UART5_RxBuffer[7];
//				param.HMI_rx[6] = UART5_RxBuffer[8];
//				param.HMI_rx[7] = UART5_RxBuffer[9];
//				param.HMI_rx[8] = UART5_RxBuffer[10];
//				param.HMI_rx[9] = UART5_RxBuffer[11];
//				param.HMI_rx[10] = UART5_RxBuffer[12];
//				param.HMI_rx[11] = UART5_RxBuffer[13];
				
				HMI_RxFlag = 1;
				
				// 重置状态机
				RxState = 0;
				RxCounter = 0;
			}
		break;
	}
}

/* 接收中断 */
void UART5_IRQHandler(void)
{
	if (USART_GetITStatus(UART5, USART_IT_RXNE) == SET)		//判断是否是UART5的接收事件触发的中断
	{
		UART5_RxData = USART_ReceiveData(UART5);				//读取数据寄存器，存放在接收的数据变量
		
		UART5_Receive_Data(UART5_RxData);

		UART5_RxFlag = 1;										//置接收标志位变量为1
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);			//清除UART5的RXNE标志位
	}
}
/*********************************** 初始化 ***********************************/
void UART5_Init(uint32_t BaudRate)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//开启UART5的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//开启GPIOC的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	//开启GPIOD的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//将PC12初始化为复用推挽输出
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);					//将PD2引脚初始化为上拉输入
	
	/*USART初始化*/
	USART_InitTypeDef USART_InitStructure;					//定义结构体变量
	USART_InitStructure.USART_BaudRate = BaudRate;			//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制，不需要
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//模式，发送模式和接收模式均选择
	USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验，不需要
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位，选择1位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长，选择8位
	USART_Init(UART5, &USART_InitStructure);				//将结构体变量交给USART_Init，配置USART1
	
	/*中断输出配置*/
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);			//开启串口接收数据的中断
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//配置NVIC为分组2
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;					//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;		//选择配置NVIC的USART1线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;		//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);							//将结构体变量交给NVIC_Init，配置NVIC外设
	
	/*USART使能*/
	USART_Cmd(UART5, ENABLE);								//使能UART5，串口开始运行
	
}



/*********************************** 接收 ***********************************/
/**
  * 函    数：获取串口接收标志位
  * 参    数：无
  * 返 回 值：串口接收标志位，范围：0~1，接收到数据后，标志位置1，读取后标志位自动清零
  */
uint8_t UART5_GetRxFlag(void)
{
	if (UART5_RxFlag == 1)			//如果标志位为1
	{
		UART5_RxFlag = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}

/**
  * 函    数：获取串口接收的数据
  * 参    数：无
  * 返 回 值：接收的数据，范围：0~255
  */
uint8_t UART5_GetRxData(void)
{
	return UART5_RxData;			//返回接收的数据变量
}


/*********************************** 发送 ***********************************/


/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void UART5_SendByte(uint8_t Byte)
{
	USART_SendData(UART5, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

/**
  * 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void UART5_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		UART5_SendByte(Array[i]);		//依次调用UART5_SendByte发送每个字节数据
	}
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void UART5_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		UART5_SendByte(String[i]);		//依次调用UART5_SendByte发送每个字节数据
	}
}

/**
  * 函    数：次方函数（内部使用）
  * 返 回 值：返回值等于X的Y次方
  */
uint32_t UART5_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//设置结果初值为1
	while (Y --)			//执行Y次
	{
		Result *= X;		//将X累乘到结果
	}
	return Result;
}

/**
  * 函    数：串口发送数字
  * 参    数：Number 要发送的数字，范围：0~4294967295
  * 参    数：Length 要发送数字的长度，范围：0~10
  * 返 回 值：无
  */
void UART5_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//根据数字长度遍历数字的每一位
	{
		UART5_SendByte(Number / UART5_Pow(10, Length - i - 1) % 10 + '0');	//依次调用Serial_SendByte发送每位数字
	}
}


/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void UART5_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	UART5_SendString(String);		//串口发送字符数组（字符串）
}

/*********************************** 功能 ***********************************/

