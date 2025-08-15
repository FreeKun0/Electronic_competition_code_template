#include "UART4.h"

uint8_t UART4_RxData;		//���崮�ڽ��յ����ݱ���
uint8_t UART4_RxFlag;		//���崮�ڽ��յı�־λ����

/* ���� */
uint8_t UART4SendBuf[5] = {0x52, 0x03, 0x00, 0x00, 0x00};
void UART4_SendData(void)
{
	UART4SendBuf[0] = 0x52;	//֡ͷ0x52
	UART4SendBuf[1] = 0x03;	/* �ǵøĳ���֡ */
	
	UART4SendBuf[2] = 0x22;
	UART4SendBuf[3] = 0x33;
	UART4SendBuf[4] = 0x44;

	UART4_SendArray(UART4SendBuf, 5);
}

/* ���� */
uint16_t UART4_RxBuffer[UART4_RX_SIZE] = {0};
uint8_t UART4_RxErr = 0;

void UART4_Receive_Data(uint8_t com_data)
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
			UART4_RxBuffer[RxCounter++] = com_data;  // �洢֡ͷ
		}
		break;

		case 1:  // ���ճ����ֶ�
			packet_length = com_data + 2;  // ���ݳ��� + ֡ͷ(1) + ����(1)
			if(packet_length > UART4_RX_SIZE)
			{
				RxState = 0;
				UART4_RxErr = 1;  // �����ȳ���������
				for(uint8_t i = 0; i < UART4_RX_SIZE; i++)
				{
					UART4_RxBuffer[i] = 0;
				}
				break;
			}
			RxState = 2;
			UART4_RxBuffer[RxCounter++] = com_data;	//�洢�����ֶ�
		break;

		case 2:  // ��������
			UART4_RxBuffer[RxCounter++] = com_data;
			if(RxCounter >= packet_length)
			{
//				param.uart_rx[0] = UART4_RxBuffer[2];
//				param.uart_rx[1] = UART4_RxBuffer[3];
//				param.uart_rx[2] = UART4_RxBuffer[4];
//				param.uart_rx[3] = UART4_RxBuffer[5];
				
				// ����״̬��
				RxState = 0;
				RxCounter = 0;
			}
		break;
	}
}

/* �����ж� */
void UART4_IRQHandler(void)
{
	if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET)		//�ж��Ƿ���UART4�Ľ����¼��������ж�
	{
		UART4_RxData = USART_ReceiveData(UART4);				//��ȡ���ݼĴ���������ڽ��յ����ݱ���
		
		UART4_Receive_Data(UART4_RxData);

		UART4_RxFlag = 1;										//�ý��ձ�־λ����Ϊ1
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);			//���UART4��RXNE��־λ
	}
}
/*********************************** ��ʼ�� ***********************************/
void UART4_Init(uint32_t BaudRate)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//����UART4��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//����GPIOC��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//��PC12��ʼ��Ϊ�����������
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//��PD2���ų�ʼ��Ϊ��������
	
	/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate = BaudRate;			//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ���ѡ��8λ
	USART_Init(UART4, &USART_InitStructure);				//���ṹ���������USART_Init������USART1
	
	/*�ж��������*/
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);			//�������ڽ������ݵ��ж�
	
	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//����NVICΪ����2
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;		//ѡ������NVIC��USART1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;		//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);							//���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(UART4, ENABLE);								//ʹ��UART4�����ڿ�ʼ����
	
}



/*********************************** ���� ***********************************/
/**
  * ��    ������ȡ���ڽ��ձ�־λ
  * ��    ������
  * �� �� ֵ�����ڽ��ձ�־λ����Χ��0~1�����յ����ݺ󣬱�־λ��1����ȡ���־λ�Զ�����
  */
uint8_t UART4_GetRxFlag(void)
{
	if (UART4_RxFlag == 1)			//�����־λΪ1
	{
		UART4_RxFlag = 0;
		return 1;					//�򷵻�1�����Զ������־λ
	}
	return 0;						//�����־λΪ0���򷵻�0
}

/**
  * ��    ������ȡ���ڽ��յ�����
  * ��    ������
  * �� �� ֵ�����յ����ݣ���Χ��0~255
  */
uint8_t UART4_GetRxData(void)
{
	return UART4_RxData;			//���ؽ��յ����ݱ���
}


/*********************************** ���� ***********************************/


/**
  * ��    �������ڷ���һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ�
  * �� �� ֵ����
  */
void UART4_SendByte(uint8_t Byte)
{
	USART_SendData(UART4, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

/**
  * ��    �������ڷ���һ������
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void UART4_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		UART4_SendByte(Array[i]);		//���ε���UART4_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void UART4_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		UART4_SendByte(String[i]);		//���ε���UART4_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �����η��������ڲ�ʹ�ã�
  * �� �� ֵ������ֵ����X��Y�η�
  */
uint32_t UART4_Pow(uint32_t X, uint32_t Y)
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
void UART4_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//�������ֳ��ȱ������ֵ�ÿһλ
	{
		UART4_SendByte(Number / UART4_Pow(10, Length - i - 1) % 10 + '0');	//���ε���Serial_SendByte����ÿλ����
	}
}


/**
  * ��    �����Լ���װ��prinf����
  * ��    ����format ��ʽ���ַ���
  * ��    ����... �ɱ�Ĳ����б�
  * �� �� ֵ����
  */
void UART4_Printf(char *format, ...)
{
	char String[100];				//�����ַ�����
	va_list arg;					//����ɱ�����б��������͵ı���arg
	va_start(arg, format);			//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);	//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);					//��������arg
	UART4_SendString(String);		//���ڷ����ַ����飨�ַ�����
}

/*********************************** ���� ***********************************/

