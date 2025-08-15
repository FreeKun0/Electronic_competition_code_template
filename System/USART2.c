#include "USART2.h"

uint8_t USART2_RxData;		//���崮�ڽ��յ����ݱ���
uint8_t USART2_RxFlag;		//���崮�ڽ��յı�־λ����

/* ���� */
/* ���� */
uint8_t USART2SendBuf[5] = {0x52, 0x03, 0x00, 0x00, 0x00};
void USART2_SendData(void)
{
	USART2SendBuf[0] = 0x52;	//֡ͷ0x52
	USART2SendBuf[1] = 0x03;	/* �ǵøĳ���֡ */

	USART2SendBuf[2] = 0x22;
	USART2SendBuf[3] = 0x33;
	USART2SendBuf[4] = 0x44;

	USART2_SendArray(USART2SendBuf, 5);
}

/* ���� */
uint16_t USART2_RxBuffer[USART2_RX_SIZE] = {0};
uint8_t USART2_RxErr = 0;
void USART2_Receive_Data(uint8_t com_data)
{
	static uint8_t RxCounter = 0;
	static uint8_t RxState = 0;
	static uint8_t packet_length = 0;  // �洢��ǰ���ݰ�����	

	switch(RxState)
	{
		case 0:  // �ȴ�֡ͷ0x52
		if(com_data == 0x52)
		{
			RxState = 1;
			RxCounter = 0;
			USART2_RxBuffer[RxCounter++] = com_data;  // �洢֡ͷ
		}
		break;

		case 1:  // ���ճ����ֶ�
			packet_length = com_data + 2;  // ���ݳ��� + ֡ͷ(1) + ����(1)
			if(packet_length > USART2_RX_SIZE)
			{
				RxState = 0;
				USART2_RxErr = 1;  // �����ȳ���������
				for(uint8_t i = 0; i < USART2_RX_SIZE; i++)
				{
					USART2_RxBuffer[i] = 0;
				}
				break;
			}
			RxState = 2;
			USART2_RxBuffer[RxCounter++] = com_data;	//�洢�����ֶ�
		break;

		case 2:  // ��������
			USART2_RxBuffer[RxCounter++] = com_data;
			if(RxCounter >= packet_length)
			{
//				param.uart_rx[0] = USART2_RxBuffer[2];
//				param.uart_rx[1] = USART2_RxBuffer[3];
//				param.uart_rx[2] = USART2_RxBuffer[4];
//				param.uart_rx[3] = USART2_RxBuffer[5];
				
				// ����״̬��
				RxState = 0;
				RxCounter = 0;
			}
		break;
	}
}

/* �����ж� */
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)		//�ж��Ƿ���USART2�Ľ����¼��������ж�
	{
		USART2_RxData = USART_ReceiveData(USART2);				//��ȡ���ݼĴ���������ڽ��յ����ݱ���
		
		USART2_Receive_Data(USART2_RxData);

		USART2_RxFlag = 1;										//�ý��ձ�־λ����Ϊ1
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);			//���USART2��RXNE��־λ
	}
}
/*********************************** ��ʼ�� ***********************************/
void USART2_Init(uint32_t BaudRate)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//����USART1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PB10��ʼ��Ϊ�����������
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PB11���ų�ʼ��Ϊ��������
	
	/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate = BaudRate;			//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ���ѡ��8λ
	USART_Init(USART2, &USART_InitStructure);				//���ṹ���������USART_Init������USART1
	
	/*�ж��������*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//�������ڽ������ݵ��ж�
	
	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//����NVICΪ����2
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//ѡ������NVIC��USART1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;		//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);							//���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(USART2, ENABLE);								//ʹ��USART2�����ڿ�ʼ����
}



/*********************************** ���� ***********************************/
/**
  * ��    ������ȡ���ڽ��ձ�־λ
  * ��    ������
  * �� �� ֵ�����ڽ��ձ�־λ����Χ��0~1�����յ����ݺ󣬱�־λ��1����ȡ���־λ�Զ�����
  */
uint8_t USART2_GetRxFlag(void)
{
	if (USART2_RxFlag == 1)			//�����־λΪ1
	{
		USART2_RxFlag = 0;
		return 1;					//�򷵻�1�����Զ������־λ
	}
	return 0;						//�����־λΪ0���򷵻�0
}

/**
  * ��    ������ȡ���ڽ��յ�����
  * ��    ������
  * �� �� ֵ�����յ����ݣ���Χ��0~255
  */
uint8_t USART2_GetRxData(void)
{
	return USART2_RxData;			//���ؽ��յ����ݱ���
}





/*********************************** ���� ***********************************/


void USART2_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

/**
  * ��    �������ڷ���һ������
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void USART2_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		USART2_SendByte(Array[i]);		//���ε���USART2_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void USART2_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		USART2_SendByte(String[i]);		//���ε���USART2_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �����η��������ڲ�ʹ�ã�
  * �� �� ֵ������ֵ����X��Y�η�
  */
uint32_t USART2_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//���ý����ֵΪ1
	while (Y --)			//ִ��Y��
	{
		Result *= X;		//��X�۳˵����
	}
	return Result;
}

/**
  * ��    �������ڷ�������
  * ��    ����Number Ҫ���͵����֣���Χ��0~4294967295
  * ��    ����Length Ҫ�������ֵĳ��ȣ���Χ��0~10
  * �� �� ֵ����
  */
void USART2_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//�������ֳ��ȱ������ֵ�ÿһλ
	{
		USART2_SendByte(Number / USART2_Pow(10, Length - i - 1) % 10 + '0');	//���ε���Serial_SendByte����ÿλ����
	}
}


/**
  * ��    �����Լ���װ��prinf����
  * ��    ����format ��ʽ���ַ���
  * ��    ����... �ɱ�Ĳ����б�
  * �� �� ֵ����
  */
void USART2_Printf(char *format, ...)
{
	char String[100];				//�����ַ�����
	va_list arg;					//����ɱ�����б��������͵ı���arg
	va_start(arg, format);			//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);	//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);					//��������arg
	USART2_SendString(String);		//���ڷ����ַ����飨�ַ�����
}

/*********************************** ���� ***********************************/

