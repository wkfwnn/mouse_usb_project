#include "stm32_dma.h"

extern u8 Stream_Buff[];
extern u8 test_buff[48];

void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	// ʱ������
	DMA_RCC_Configuration();

	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);			// �������ַ(�鿴stm32f10x_lib.h+�Ĵ���ƫ����)
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Stream_Buff;				    // DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;							// �趨������Ϊ���ݴ���Ŀ�Ļ�����Դ
	DMA_InitStructure.DMA_BufferSize = 0x30;								// �����С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			// �����ַ�����������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						// �ڴ��ַ�����������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	// �������ݿ��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				// �ڴ����ݿ��
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								// ���ݴ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;						// ͨ�����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						 // �ڴ浽�ڴ�ģʽʹ��

	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	// �ж�����
	DMA_NVIC_Configuration(); 
	//ʹ��
	//DMA_Cmd(DMA1_Channel4,ENABLE);
}

// ʱ������
void DMA_RCC_Configuration(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
}

// �ж�����
void DMA_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������//
//���ش�����������Ŀ
//u16 CurrDataCount = DMA_GetCurrDataCounter(DMA1_Channel1);

//FlagStatus Status = DMA_GetFlagStatus(DMA1_FLAG_GL1);
										//DMA1_FLAG_GL1	//ͨ��1ȫ�ֱ�־λ
										//DMA1_FLAG_TC1	//ͨ��1������ɱ�־λ
										//DMA1_FLAG_HT1	//ͨ��1��������־λ
										//DMA1_FLAG_TE1	//ͨ��1��������־λ
										//����
										//DMA2_FLAG_TC1	//ͨ��2������ɱ�־λ
										//DMA2_FLAG_HT1	//ͨ��2��������־λ
										//DMA2_FLAG_TE1	//ͨ��2��������־λ
										//DMA2_FLAG_GL1	//ͨ��3ȫ�ֱ�־λ
										//DMA2_FLAG_TC1	//ͨ��3������ɱ�־λ
										//DMA2_FLAG_HT1	//ͨ��3��������־λ
										//����
//�����־λ
//DMA_ClearFlag(DMA1_FLAG_GL1);
//����ж��Ƿ���
//ITStatus Status = DMA_GetITStatus(DMA1_IT_GL1);
					//DMA1_IT_GL1	//ͨ��1ȫ�ֱ�־λ
					//DMA1_IT_TC1	//ͨ��1������ɱ�־λ
					//DMA1_IT_HT1	//ͨ��1��������־λ
					//DMA1_IT_TE1	//ͨ��1��������־λ
					//����
//����жϱ�־λ
//DMA_ClearITPendingBit(DMA1_IT_GL1);
