#include "UART5.h"

uint8_t UART5_RxData;		//���崮�ڽ��յ����ݱ���
uint8_t UART5_RxFlag;		//���崮�ڽ��յı�־λ����
uint8_t HMI_RxFlag;		//���崮�ڽ��յı�־λ����
uint8_t HMI_packet_length;


char str[100];
char temp_str_to_int[100];
int temp_int;
char change_flag;
int temp_index;
uint8_t len1; // ���յ������ݳ���



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
			__set_FAULTMASK(1); //�ر������ж�
			NVIC_SystemReset(); //��λ
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
	char String[100];				//�����ַ�����
	va_list arg;					//����ɱ�����б��������͵ı���arg
	va_start(arg, format);			//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);	//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);					//��������arg
	UART5_SendString(String);		//���ڷ����ַ����飨�ַ�����
	UART5_SendString("\xff\xff\xff");
}

uint8_t HMI_GetRxFlag(void)
{
	if (HMI_RxFlag == 1)			//�����־λΪ1
	{
		HMI_RxFlag = 0;
		return 1;					//�򷵻�1�����Զ������־λ
	}
	return 0;						//�����־λΪ0���򷵻�0
}

/* ���� */
//uint8_t UART5SendBuf[5] = {0x52, 0x03, 0x00, 0x00, 0x00};
//void UART5_SendData(void)
//{
//	UART5SendBuf[0] = 0x52;	//֡ͷ0x52
//	UART5SendBuf[1] = 0x03;	/* �ǵøĳ���֡ */
//	
//	UART5SendBuf[2] = 0x22;
//	UART5SendBuf[3] = 0x33;
//	UART5SendBuf[4] = 0x44;

//	UART5_SendArray(UART5SendBuf, 5);
//}

/* ���� */
uint16_t UART5_RxBuffer[UART5_RX_SIZE] = {0};
uint8_t UART5_RxErr = 0;

void UART5_Receive_Data(uint8_t com_data)
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
			UART5_RxBuffer[RxCounter++] = com_data;  // �洢֡ͷ
		}
		break;

		case 1:  // ���ճ����ֶ�
			packet_length = com_data + 2;  // ���ݳ��� + ֡ͷ(1) + ����(1)
			if(packet_length > UART5_RX_SIZE)
			{
				RxState = 0;
				UART5_RxErr = 1;  // �����ȳ���������
				for(uint8_t i = 0; i < UART5_RX_SIZE; i++)
				{
					UART5_RxBuffer[i] = 0;
				}
				break;
			}
			RxState = 2;
			UART5_RxBuffer[RxCounter++] = com_data;	//�洢�����ֶ�
		break;

		case 2:  // ��������
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
				
				// ����״̬��
				RxState = 0;
				RxCounter = 0;
			}
		break;
	}
}

/* �����ж� */
void UART5_IRQHandler(void)
{
	if (USART_GetITStatus(UART5, USART_IT_RXNE) == SET)		//�ж��Ƿ���UART5�Ľ����¼��������ж�
	{
		UART5_RxData = USART_ReceiveData(UART5);				//��ȡ���ݼĴ���������ڽ��յ����ݱ���
		
		UART5_Receive_Data(UART5_RxData);

		UART5_RxFlag = 1;										//�ý��ձ�־λ����Ϊ1
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);			//���UART5��RXNE��־λ
	}
}
/*********************************** ��ʼ�� ***********************************/
void UART5_Init(uint32_t BaudRate)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//����UART5��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//����GPIOC��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	//����GPIOD��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//��PC12��ʼ��Ϊ�����������
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);					//��PD2���ų�ʼ��Ϊ��������
	
	/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate = BaudRate;			//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ���ѡ��8λ
	USART_Init(UART5, &USART_InitStructure);				//���ṹ���������USART_Init������USART1
	
	/*�ж��������*/
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);			//�������ڽ������ݵ��ж�
	
	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//����NVICΪ����2
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;		//ѡ������NVIC��USART1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;		//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);							//���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(UART5, ENABLE);								//ʹ��UART5�����ڿ�ʼ����
	
}



/*********************************** ���� ***********************************/
/**
  * ��    ������ȡ���ڽ��ձ�־λ
  * ��    ������
  * �� �� ֵ�����ڽ��ձ�־λ����Χ��0~1�����յ����ݺ󣬱�־λ��1����ȡ���־λ�Զ�����
  */
uint8_t UART5_GetRxFlag(void)
{
	if (UART5_RxFlag == 1)			//�����־λΪ1
	{
		UART5_RxFlag = 0;
		return 1;					//�򷵻�1�����Զ������־λ
	}
	return 0;						//�����־λΪ0���򷵻�0
}

/**
  * ��    ������ȡ���ڽ��յ�����
  * ��    ������
  * �� �� ֵ�����յ����ݣ���Χ��0~255
  */
uint8_t UART5_GetRxData(void)
{
	return UART5_RxData;			//���ؽ��յ����ݱ���
}


/*********************************** ���� ***********************************/


/**
  * ��    �������ڷ���һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ�
  * �� �� ֵ����
  */
void UART5_SendByte(uint8_t Byte)
{
	USART_SendData(UART5, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

/**
  * ��    �������ڷ���һ������
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void UART5_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		UART5_SendByte(Array[i]);		//���ε���UART5_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void UART5_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		UART5_SendByte(String[i]);		//���ε���UART5_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �����η��������ڲ�ʹ�ã�
  * �� �� ֵ������ֵ����X��Y�η�
  */
uint32_t UART5_Pow(uint32_t X, uint32_t Y)
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
void UART5_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//�������ֳ��ȱ������ֵ�ÿһλ
	{
		UART5_SendByte(Number / UART5_Pow(10, Length - i - 1) % 10 + '0');	//���ε���Serial_SendByte����ÿλ����
	}
}


/**
  * ��    �����Լ���װ��prinf����
  * ��    ����format ��ʽ���ַ���
  * ��    ����... �ɱ�Ĳ����б�
  * �� �� ֵ����
  */
void UART5_Printf(char *format, ...)
{
	char String[100];				//�����ַ�����
	va_list arg;					//����ɱ�����б��������͵ı���arg
	va_start(arg, format);			//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);	//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);					//��������arg
	UART5_SendString(String);		//���ڷ����ַ����飨�ַ�����
}

/*********************************** ���� ***********************************/

