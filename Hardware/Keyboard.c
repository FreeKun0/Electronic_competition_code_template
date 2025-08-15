#include "Keyboard.h"

//		1		2		5		7
//	3
//	4
//	6
//	8

//		PA11	PA12	PA0		PA15
//	PB4
//	PB5
//	PA1
//	PB3

const Key_MatrixConfig_t Key_MatrixConfig = 
{
    .RowCount = 4,			// ����
    .ColCount = 4,			// ����
    .RowPins =				// ����������
	{
        {GPIOB, GPIO_Pin_4},		// ��1
        {GPIOB, GPIO_Pin_5},		// ��2
        {GPIOA, GPIO_Pin_1},		// ��3
		{GPIOB, GPIO_Pin_3},		// ��4
        // ��������У��ɼ������
    },
    .ColPins =				// ����������
	{
        {GPIOA, GPIO_Pin_11},		// ��1
        {GPIOA, GPIO_Pin_12},		// ��2
        {GPIOA, GPIO_Pin_0},		// ��3
        {GPIOA, GPIO_Pin_15},		// ��4
		// ��������У��ɼ������
    }
};

uint8_t Keyboard_Num   = 0;

void Keyboard_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t i;
    
    // ʹ������������GPIO�˿ڵ�ʱ��
    for(i = 0; i < Key_MatrixConfig.RowCount; i++) 
	{
        if(Key_MatrixConfig.RowPins[i].GPIOx == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        else if(Key_MatrixConfig.RowPins[i].GPIOx == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        else if(Key_MatrixConfig.RowPins[i].GPIOx == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        else if(Key_MatrixConfig.RowPins[i].GPIOx == GPIOD) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
        else if(Key_MatrixConfig.RowPins[i].GPIOx == GPIOE) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    }
    
    // ʹ������������GPIO�˿ڵ�ʱ��
    for(i = 0; i < Key_MatrixConfig.ColCount; i++)
	{
        if(Key_MatrixConfig.ColPins[i].GPIOx == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        else if(Key_MatrixConfig.ColPins[i].GPIOx == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        else if(Key_MatrixConfig.ColPins[i].GPIOx == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        else if(Key_MatrixConfig.ColPins[i].GPIOx == GPIOD) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
        else if(Key_MatrixConfig.ColPins[i].GPIOx == GPIOE) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    }
    
    // ����������Ϊ��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    for(i = 0; i < Key_MatrixConfig.RowCount; i++)
	{
        GPIO_InitStructure.GPIO_Pin = Key_MatrixConfig.RowPins[i].Pin;
        GPIO_Init(Key_MatrixConfig.RowPins[i].GPIOx, &GPIO_InitStructure);
    }
    
    // ����������Ϊ�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    
    for(i = 0; i < Key_MatrixConfig.ColCount; i++)
	{
        GPIO_InitStructure.GPIO_Pin = Key_MatrixConfig.ColPins[i].Pin;
        GPIO_Init(Key_MatrixConfig.ColPins[i].GPIOx, &GPIO_InitStructure);
    }
    
    // ��ʼ״̬����������ߵ�ƽ
    for(i = 0; i < Key_MatrixConfig.ColCount; i++) 
	{
        GPIO_SetBits(Key_MatrixConfig.ColPins[i].GPIOx, Key_MatrixConfig.ColPins[i].Pin);
    }
}

uint8_t Keyboard_GetNum(void)
{
    uint8_t Temp;
	if(Keyboard_Num)
	{
		Temp = Keyboard_Num;
		Keyboard_Num = 0;
		return Temp;
	}
	return 0;
}


uint8_t Keyboard_Getstate(void)
{
    uint8_t col, row, key = 0;
    uint8_t row_state[MAX_ROW_COUNT];
    
    // ����ɨ��
    for(col = 0; col < Key_MatrixConfig.ColCount; col++)
    {
        // ���õ�ǰ��Ϊ�͵�ƽ��������Ϊ�ߵ�ƽ
        for(int i = 0; i < Key_MatrixConfig.ColCount; i++)
        {
            if(i == col)
                GPIO_ResetBits(Key_MatrixConfig.ColPins[i].GPIOx, Key_MatrixConfig.ColPins[i].Pin);
            else
                GPIO_SetBits(Key_MatrixConfig.ColPins[i].GPIOx, Key_MatrixConfig.ColPins[i].Pin);
        }
        
        // ������ʱ�����ȶ���ƽ
//        for(volatile int i = 0; i < 100; i++);
        
        // ��ȡ�����е�״̬
        for(row = 0; row < Key_MatrixConfig.RowCount; row++) 
        {
            row_state[row] = GPIO_ReadInputDataBit(Key_MatrixConfig.RowPins[row].GPIOx, Key_MatrixConfig.RowPins[row].Pin);
        }
        
        // ����Ƿ����б�����
        for(row = 0; row < Key_MatrixConfig.RowCount; row++) 
        {
            if(row_state[row] == 0) 
            {
                // �����ֵ (1-16)
                key = row * Key_MatrixConfig.ColCount+ (col + 1);
                break;
            }
        }
        if(key != 0) break; // ��⵽�������а�������������ѭ��
    }
    // �ָ�������Ϊ�͵�ƽ
    for(int i = 0; i < Key_MatrixConfig.ColCount; i++)
    {
        GPIO_SetBits(Key_MatrixConfig.ColPins[i].GPIOx, Key_MatrixConfig.ColPins[i].Pin);
    }
    return key;
}

void Keyboard_Tick(void)
{
    static uint8_t Count;
    static uint8_t CurrState, PrevState;
    
    Count++;
    if (Count >= 20)  //��ʱ��1ms,����20ms
    {
        Count = 0;
        PrevState = CurrState;
        CurrState = Keyboard_Getstate();
        
		if (CurrState == 0 && PrevState != 0)
		{
			Keyboard_Num = PrevState;
		}
    }
}
