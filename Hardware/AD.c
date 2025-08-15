#include "AD.h"



uint8_t Load_flag = 0;  // 0��ʾ��ûת��ҩƷ����ʾת����ϣ�-1��ʾҩƷ��

uint8_t LoadCount, NotLoadCount;

float AD_Voltage;


/**
  * ��    ����AD��ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void AD_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//����ADC1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//����GPIOA��ʱ��
	
	/*����ADCʱ��*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);						//ѡ��ʱ��6��Ƶ��ADCCLK = 72MHz / 6 = 12MHz
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				//CH14
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				//CH15
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;	//CH14 CH15
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//��PC4��PC5���ų�ʼ��Ϊģ������
	
	/*���ڴ˴����ù��������У�������ÿ��ADת��ǰ���ã���������������ADת����ͨ��*/
	
	/*ADC��ʼ��*/
	ADC_InitTypeDef ADC_InitStructure;						//����ṹ�����
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//ģʽ��ѡ�����ģʽ��������ʹ��ADC1
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//���ݶ��룬ѡ���Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�ⲿ������ʹ���������������Ҫ�ⲿ����
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//����ת����ʧ�ܣ�ÿת��һ�ι��������к�ֹͣ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//ɨ��ģʽ��ʧ�ܣ�ֻת�������������1��һ��λ��
	ADC_InitStructure.ADC_NbrOfChannel = 1;					//ͨ������Ϊ1������ɨ��ģʽ�£�����Ҫָ������1�������ڷ�ɨ��ģʽ�£�ֻ����1
	ADC_Init(ADC1, &ADC_InitStructure);						//���ṹ���������ADC_Init������ADC1
	
	/*ADCʹ��*/
	ADC_Cmd(ADC1, ENABLE);									//ʹ��ADC1��ADC��ʼ����
	
	/*ADCУ׼*/
	ADC_ResetCalibration(ADC1);								//�̶����̣��ڲ��е�·���Զ�ִ��У׼
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

/**
  * ��    ������ȡADת����ֵ
  * ��    ����ADC_Channel ָ��ADת����ͨ������Χ��ADC_Channel_x������x������14/15
  * �� �� ֵ��ADת����ֵ����Χ��0~4095
  */
uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);	//��ÿ��ת��ǰ�����ݺ����β������Ĺ������ͨ��1
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);					//�������ADת��һ��
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);	//�ȴ�EOC��־λ�����ȴ�ADת������
	return ADC_GetConversionValue(ADC1);					//�����ݼĴ������õ�ADת���Ľ��
}
float AD_GetVoltage(uint8_t ADC_Channel)
{
	uint16_t AD_Value;			//����ADֵ����
	float AD_Voltage;			//�����ѹ����
	
	AD_Value = AD_GetValue(ADC_Channel);			//��ȡADת����ֵ
	AD_Voltage = (float)AD_Value / 4095 * 3.3;		//��ADֵ���Ա任��0~3.3�ķ�Χ����ʾ��ѹ
	return AD_Voltage;
}



void LoadOrNot()
{
	AD_Voltage = AD_GetVoltage(ADC_Channel_14);

	if(AD_Voltage >= 1.6)
	{
		NotLoadCount = 0;
		
		LoadCount++;
		if(LoadCount > 3) //3�γ���ִ�е���֮���ٴ��ж�
		{
			Load_flag = 1;
		}
	}
	else if(AD_Voltage < 1.6)
	{
		LoadCount = 0;
		
		if(Load_flag == 1)    //������װ�ع�ҩƷ������²����ж��Ƿ�����ҩƷ
		{
			NotLoadCount++;
			if(NotLoadCount > 3 )  //3�γ���ִ�е���֮���ٴ��ж�
			{
				Load_flag = 2;
			}
	  }
	}
}
