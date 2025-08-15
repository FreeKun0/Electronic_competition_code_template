#include "USART2.h"

uint8_t USART2_RxData;		//定义串口接收的数据变量
uint8_t USART2_RxFlag;		//定义串口接收的标志位变量

/* 发送 */
/* 发送 */
uint8_t USART2SendBuf[5] = {0x52, 0x03, 0x00, 0x00, 0x00};
void USART2_SendData(void)
{
	USART2SendBuf[0] = 0x52;	//帧头0x52
	USART2SendBuf[1] = 0x03;	/* 记得改长度帧 */

	USART2SendBuf[2] = 0x22;
	USART2SendBuf[3] = 0x33;
	USART2SendBuf[4] = 0x44;

	USART2_SendArray(USART2SendBuf, 5);
}

/* 接收 */
uint16_t USART2_RxBuffer[USART2_RX_SIZE] = {0};
uint8_t USART2_RxErr = 0;
void USART2_Receive_Data(uint8_t com_data)
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
			USART2_RxBuffer[RxCounter++] = com_data;  // 存储帧头
		}
		break;

		case 1:  // 接收长度字段
			packet_length = com_data + 2;  // 数据长度 + 帧头(1) + 长度(1)
			if(packet_length > USART2_RX_SIZE)
			{
				RxState = 0;
				USART2_RxErr = 1;  // 包长度超过缓冲区
				for(uint8_t i = 0; i < USART2_RX_SIZE; i++)
				{
					USART2_RxBuffer[i] = 0;
				}
				break;
			}
			RxState = 2;
			USART2_RxBuffer[RxCounter++] = com_data;	//存储长度字段
		break;

		case 2:  // 接收数据
			USART2_RxBuffer[RxCounter++] = com_data;
			if(RxCounter >= packet_length)
			{
//				param.uart_rx[0] = USART2_RxBuffer[2];
//				param.uart_rx[1] = USART2_RxBuffer[3];
//				param.uart_rx[2] = USART2_RxBuffer[4];
//				param.uart_rx[3] = USART2_RxBuffer[5];
				
				// 重置状态机
				RxState = 0;
				RxCounter = 0;
			}
		break;
	}
}

/* 接收中断 */
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)		//判断是否是USART2的接收事件触发的中断
	{
		USART2_RxData = USART_ReceiveData(USART2);				//读取数据寄存器，存放在接收的数据变量
		
		USART2_Receive_Data(USART2_RxData);

		USART2_RxFlag = 1;										//置接收标志位变量为1
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);			//清除USART2的RXNE标志位
	}
}
/*********************************** 初始化 ***********************************/
void USART2_Init(uint32_t BaudRate)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PB10初始化为复用推挽输出
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PB11引脚初始化为上拉输入
	
	/*USART初始化*/
	USART_InitTypeDef USART_InitStructure;					//定义结构体变量
	USART_InitStructure.USART_BaudRate = BaudRate;			//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制，不需要
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//模式，发送模式和接收模式均选择
	USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验，不需要
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位，选择1位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长，选择8位
	USART_Init(USART2, &USART_InitStructure);				//将结构体变量交给USART_Init，配置USART1
	
	/*中断输出配置*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//开启串口接收数据的中断
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//配置NVIC为分组2
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;					//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//选择配置NVIC的USART1线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;		//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);							//将结构体变量交给NVIC_Init，配置NVIC外设
	
	/*USART使能*/
	USART_Cmd(USART2, ENABLE);								//使能USART2，串口开始运行
}



/*********************************** 接收 ***********************************/
/**
  * 函    数：获取串口接收标志位
  * 参    数：无
  * 返 回 值：串口接收标志位，范围：0~1，接收到数据后，标志位置1，读取后标志位自动清零
  */
uint8_t USART2_GetRxFlag(void)
{
	if (USART2_RxFlag == 1)			//如果标志位为1
	{
		USART2_RxFlag = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}

/**
  * 函    数：获取串口接收的数据
  * 参    数：无
  * 返 回 值：接收的数据，范围：0~255
  */
uint8_t USART2_GetRxData(void)
{
	return USART2_RxData;			//返回接收的数据变量
}





/*********************************** 发送 ***********************************/


void USART2_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

/**
  * 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void USART2_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		USART2_SendByte(Array[i]);		//依次调用USART2_SendByte发送每个字节数据
	}
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void USART2_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		USART2_SendByte(String[i]);		//依次调用USART2_SendByte发送每个字节数据
	}
}

/**
  * 函    数：次方函数（内部使用）
  * 返 回 值：返回值等于X的Y次方
  */
uint32_t USART2_Pow(uint32_t X, uint32_t Y)
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
void USART2_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//根据数字长度遍历数字的每一位
	{
		USART2_SendByte(Number / USART2_Pow(10, Length - i - 1) % 10 + '0');	//依次调用Serial_SendByte发送每位数字
	}
}


/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void USART2_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	USART2_SendString(String);		//串口发送字符数组（字符串）
}

/*********************************** 功能 ***********************************/

