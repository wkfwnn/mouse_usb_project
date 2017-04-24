#include "stm32_dma.h"

extern u8 Stream_Buff[];
extern u8 test_buff[48];

void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	// 时钟配置
	DMA_RCC_Configuration();

	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);			// 外设基地址(查看stm32f10x_lib.h+寄存器偏移量)
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Stream_Buff;				    // DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;							// 设定外设作为数据传输目的还是来源
	DMA_InitStructure.DMA_BufferSize = 0x30;								// 缓存大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			// 外设地址递增允许控制
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						// 内存地址递增允许控制
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	// 外设数据宽度
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				// 内存数据宽度
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								// 数据传输模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;						// 通道优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						 // 内存到内存模式使能

	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	// 中断配置
	DMA_NVIC_Configuration(); 
	//使能
	//DMA_Cmd(DMA1_Channel4,ENABLE);
}

// 时钟配置
void DMA_RCC_Configuration(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
}

// 中断配置
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

//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓其他函数↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓//
//返回待传输数据数目
//u16 CurrDataCount = DMA_GetCurrDataCounter(DMA1_Channel1);

//FlagStatus Status = DMA_GetFlagStatus(DMA1_FLAG_GL1);
										//DMA1_FLAG_GL1	//通道1全局标志位
										//DMA1_FLAG_TC1	//通道1传输完成标志位
										//DMA1_FLAG_HT1	//通道1传输过半标志位
										//DMA1_FLAG_TE1	//通道1传输错误标志位
										//……
										//DMA2_FLAG_TC1	//通道2传输完成标志位
										//DMA2_FLAG_HT1	//通道2传输过半标志位
										//DMA2_FLAG_TE1	//通道2传输错误标志位
										//DMA2_FLAG_GL1	//通道3全局标志位
										//DMA2_FLAG_TC1	//通道3传输完成标志位
										//DMA2_FLAG_HT1	//通道3传输过半标志位
										//……
//清除标志位
//DMA_ClearFlag(DMA1_FLAG_GL1);
//检测中断是否发生
//ITStatus Status = DMA_GetITStatus(DMA1_IT_GL1);
					//DMA1_IT_GL1	//通道1全局标志位
					//DMA1_IT_TC1	//通道1传输完成标志位
					//DMA1_IT_HT1	//通道1传输过半标志位
					//DMA1_IT_TE1	//通道1传输错误标志位
					//……
//清除中断标志位
//DMA_ClearITPendingBit(DMA1_IT_GL1);
