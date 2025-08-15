#include "k210.h"

uint8_t USART3_RxData;		//���崮�ڽ��յ����ݱ���
uint8_t USART3_RxFlag;		//���崮�ڽ��յı�־λ����

/* ���� */
/* ���� */
uint8_t USART3SendBuf[5] = {0x52, 0x03, 0x00, 0x00, 0x00};
void USART3_SendData(void)
{
	USART3SendBuf[0] = 0x52;	//֡ͷ0x52
	USART3SendBuf[1] = 0x03;	/* �ǵøĳ���֡ */

	USART3SendBuf[2] = 0x22;
	USART3SendBuf[3] = 0x33;
	USART3SendBuf[4] = 0x44;

	USART3_SendArray(USART3SendBuf, 5);
}

void K210_SendByte(uint8_t Byte)
{
	USART3_SendByte(Byte);
}

/* ���� */
uint16_t USART3_RxBuffer[USART3_RX_SIZE] = {0};
uint8_t USART3_RxErr = 0;
void USART3_Receive_Data(uint8_t com_data)
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
			USART3_RxBuffer[RxCounter++] = com_data;  // �洢֡ͷ
		}
		break;

		case 1:  // ���ճ����ֶ�
			packet_length = com_data + 2;  // ���ݳ��� + ֡ͷ(1) + ����(1)
			if(packet_length > USART3_RX_SIZE)
			{
				RxState = 0;
				USART3_RxErr = 1;  // �����ȳ���������
				for(uint8_t i = 0; i < USART3_RX_SIZE; i++)
				{
					USART3_RxBuffer[i] = 0;
				}
				break;
			}
			RxState = 2;
			USART3_RxBuffer[RxCounter++] = com_data;	//�洢�����ֶ�
		break;

		case 2:  // ��������
			USART3_RxBuffer[RxCounter++] = com_data;
			if(RxCounter >= packet_length)
			{
//				param.uart_rx[0] = USART3_RxBuffer[2];
//				param.uart_rx[1] = USART3_RxBuffer[3];
//				param.uart_rx[2] = USART3_RxBuffer[4];
//				param.uart_rx[3] = USART3_RxBuffer[5];
				
				if(TASK == 1)	//����������
				{
	//				pencil_points[count.pencil_count][0] = USART3_RxBuffer[RxCounter - 8] + USART3_RxBuffer[RxCounter - 6];
	//				pencil_points[count.pencil_count][1] = USART3_RxBuffer[RxCounter - 7] + USART3_RxBuffer[RxCounter - 5];
					
					pencil_points[count.pencil_count][0] = USART3_RxBuffer[RxCounter - 4] + USART3_RxBuffer[RxCounter - 2];
					pencil_points[count.pencil_count][1] = USART3_RxBuffer[RxCounter - 3];
					
					flag.get_one_pencil_succeed_flag = 1;
				}
				else if(TASK == 2)	//��λ
				{
	//				//ǰ������ɫ��������
	//				C_red_last_last[0] = C_red_last[0];
	//				C_red_last_last[1] = C_red_last[1];
	//				
	//				//ǰһ����ɫ��������
	//				C_red_last[0] = C_red[0];
	//				C_red_last[1] = C_red[1];
					
					//��ǰ��ɫ��������
	//				C_red[0] = USART3_RxBuffer[RxCounter - 8] + USART3_RxBuffer[RxCounter - 6];
	//				C_red[1] = USART3_RxBuffer[RxCounter - 7] + USART3_RxBuffer[RxCounter - 5];
					
					C_red[0] = USART3_RxBuffer[RxCounter - 4] + USART3_RxBuffer[RxCounter - 2];
					C_red[1] = USART3_RxBuffer[RxCounter - 3];
					
				}
				else if(TASK == 3) //ѲǦ����
				{
	//				//ǰ������ɫ��������
	//				C_red_last_last[0] = C_red_last[0];
	//				C_red_last_last[1] = C_red_last[1];
	//				
	//				//ǰһ����ɫ��������
	//				C_red_last[0] = C_red[0];
	//				C_red_last[1] = C_red[1];
					
					//��ǰ��ɫ��������
	//				C_red[0] = USART3_RxBuffer[RxCounter - 8] + USART3_RxBuffer[RxCounter - 6];
	//				C_red[1] = USART3_RxBuffer[RxCounter - 7] + USART3_RxBuffer[RxCounter - 5];
					
					C_red[0] = USART3_RxBuffer[RxCounter - 4] + USART3_RxBuffer[RxCounter - 2];
					C_red[1] = USART3_RxBuffer[RxCounter - 3];
				}
				else if(TASK == 4)	//
				{
					
				}
				else if(TASK == 5)	//
				{
					
				}
				else if(TASK == 6)	//��ȡ�ڿ�ǵ�
				{
					if(flag.black_pionts_get_flag == 1)	//����ȴ�����
					{
						black_points[0][0] = USART3_RxBuffer[RxCounter - 13] + USART3_RxBuffer[RxCounter - 11];
						black_points[0][1] = USART3_RxBuffer[RxCounter - 12];
						
						black_points[1][0] = USART3_RxBuffer[RxCounter - 10] + USART3_RxBuffer[RxCounter - 8];
						black_points[1][1] = USART3_RxBuffer[RxCounter - 9];
						
						black_points[2][0] = USART3_RxBuffer[RxCounter - 7] + USART3_RxBuffer[RxCounter - 5];
						black_points[2][1] = USART3_RxBuffer[RxCounter - 6];
						
						black_points[3][0] = USART3_RxBuffer[RxCounter - 4] + USART3_RxBuffer[RxCounter - 2];
						black_points[3][1] = USART3_RxBuffer[RxCounter - 3];
						
						flag.black_pionts_get_flag = 2;	//�ɹ�����
					}
				}
				else if(TASK == 7)	//�ߺڿ�
				{
	//				//ǰ������ɫ��������
	//				C_red_last_last[0] = C_red_last[0];
	//				C_red_last_last[1] = C_red_last[1];
	//				
	//				//ǰһ����ɫ��������
	//				C_red_last[0] = C_red[0];
	//				C_red_last[1] = C_red[1];
					
					//��ǰ��ɫ��������
					C_red[0] = USART3_RxBuffer[RxCounter - 4] + USART3_RxBuffer[RxCounter - 2];
					C_red[1] = USART3_RxBuffer[RxCounter - 3];
				}
	//			else if(TASK == 8)	//׷��
	//			{
	//				//��ǰ��ɫ��������
	//				C_red[0] = USART3_RxBuffer[RxCounter - 7] + USART3_RxBuffer[RxCounter - 5];
	//				C_red[1] = USART3_RxBuffer[RxCounter - 6];
	//				
	//				//��ǰ��ɫ��������
	//				C_green[0] = USART3_RxBuffer[RxCounter - 4] + USART3_RxBuffer[RxCounter - 2];
	//				C_green[1] = USART3_RxBuffer[RxCounter - 3];
	//			}
	//			else if(TASK == 9)	//׷��
	//			{
	//				//��ǰ��ɫ��������
	//				C_red[0] = USART3_RxBuffer[RxCounter - 7] + USART3_RxBuffer[RxCounter - 5];
	//				C_red[1] = USART3_RxBuffer[RxCounter - 6];
	//				
	//				//��ǰ��ɫ��������
	//				C_green[0] = USART3_RxBuffer[RxCounter - 4] + USART3_RxBuffer[RxCounter - 2];
	//				C_green[1] = USART3_RxBuffer[RxCounter - 3];
	//			}
				else if(TASK == 10)	//Ŀ���ʵ�ʵ�
				{
//					flag.cur_catch_tar_flag = USART3_RxBuffer[RxCounter - 8];
//					//��ǰ��ɫ��������
//					C_red[0] = USART3_RxBuffer[RxCounter - 7] + USART3_RxBuffer[RxCounter - 5];
//					C_red[1] = USART3_RxBuffer[RxCounter - 6];
//					
//					//��ǰ��ɫ��������
//					C_tar[0] = USART3_RxBuffer[RxCounter - 4] + USART3_RxBuffer[RxCounter - 2];
//					C_tar[1] = USART3_RxBuffer[RxCounter - 3];
					
					if(USART3_RxBuffer[2] == 0x00)
					{
						flag.cur_catch_tar_flag = 0;
					}
					else if(USART3_RxBuffer[2] == 0x01)
					{
						flag.cur_catch_tar_flag = 1;
					}
;
					//��ǰ��ɫ��������
					C_red[0] = USART3_RxBuffer[3] + USART3_RxBuffer[5];
					C_red[1] = USART3_RxBuffer[4];
					
					//��ǰ��ɫ��������
					C_tar[0] = USART3_RxBuffer[6] + USART3_RxBuffer[8];
					C_tar[1] = USART3_RxBuffer[7];
				}
				//���������������USART3_RxBuffer[RxCounter - 1]�ŵ���֡β
					
				// ����״̬��
				RxState = 0;
				RxCounter = 0;
			}
		break;
	}
}

/* �����ж� */
void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)		//�ж��Ƿ���USART3�Ľ����¼��������ж�
	{
		USART3_RxData = USART_ReceiveData(USART3);				//��ȡ���ݼĴ���������ڽ��յ����ݱ���
		
		USART3_Receive_Data(USART3_RxData);

		USART3_RxFlag = 1;										//�ý��ձ�־λ����Ϊ1
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);			//���USART3��RXNE��־λ
	}
}
/*********************************** ��ʼ�� ***********************************/
void USART3_Init(uint32_t BaudRate)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//����USART1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��PB10��ʼ��Ϊ�����������
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��PB11���ų�ʼ��Ϊ��������
	
	/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate = BaudRate;			//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ���ѡ��8λ
	USART_Init(USART3, &USART_InitStructure);				//���ṹ���������USART_Init������USART1
	
	/*�ж��������*/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//�������ڽ������ݵ��ж�
	
	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//����NVICΪ����2
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		//ѡ������NVIC��USART1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;		//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);							//���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(USART3, ENABLE);								//ʹ��USART3�����ڿ�ʼ����
	
//	/* ��K210�ⲿ�ж� */
//#if (JING_or_DONG == 1) //��̬ʶ�����жϸ���K210��ʼʶ���³���cam��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//Ĭ�ϵ͵�ƽ
//#endif
}



/*********************************** ���� ***********************************/
/**
  * ��    ������ȡ���ڽ��ձ�־λ
  * ��    ������
  * �� �� ֵ�����ڽ��ձ�־λ����Χ��0~1�����յ����ݺ󣬱�־λ��1����ȡ���־λ�Զ�����
  */
uint8_t USART3_GetRxFlag(void)
{
	if (USART3_RxFlag == 1)			//�����־λΪ1
	{
		USART3_RxFlag = 0;
		return 1;					//�򷵻�1�����Զ������־λ
	}
	return 0;						//�����־λΪ0���򷵻�0
}

/**
  * ��    ������ȡ���ڽ��յ�����
  * ��    ������
  * �� �� ֵ�����յ����ݣ���Χ��0~255
  */
uint8_t USART3_GetRxData(void)
{
	return USART3_RxData;			//���ؽ��յ����ݱ���
}





/*********************************** ���� ***********************************/



/**
  * ��    �������ڷ���һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ�
  * �� �� ֵ����
  */
void USART3_SendByte(uint8_t Byte)
{
	USART_SendData(USART3, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

/**
  * ��    �������ڷ���һ������
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void USART3_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		USART3_SendByte(Array[i]);		//���ε���USART3_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void USART3_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		USART3_SendByte(String[i]);		//���ε���USART3_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �����η��������ڲ�ʹ�ã�
  * �� �� ֵ������ֵ����X��Y�η�
  */
uint32_t USART3_Pow(uint32_t X, uint32_t Y)
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
void USART3_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//�������ֳ��ȱ������ֵ�ÿһλ
	{
		USART3_SendByte(Number / USART3_Pow(10, Length - i - 1) % 10 + '0');	//���ε���Serial_SendByte����ÿλ����
	}
}


/**
  * ��    �����Լ���װ��prinf����
  * ��    ����format ��ʽ���ַ���
  * ��    ����... �ɱ�Ĳ����б�
  * �� �� ֵ����
  */
void USART3_Printf(char *format, ...)
{
	char String[100];				//�����ַ�����
	va_list arg;					//����ɱ�����б��������͵ı���arg
	va_start(arg, format);			//��format��ʼ�����ղ����б�arg����
	vsprintf(String, format, arg);	//ʹ��vsprintf��ӡ��ʽ���ַ����Ͳ����б��ַ�������
	va_end(arg);					//��������arg
	USART3_SendString(String);		//���ڷ����ַ����飨�ַ�����
}

/*********************************** ���� ***********************************/

