#include "stm32_usart.h"


FlagStatus USART_SendEnd = RESET;	// 中断发送结束标志(SET:未发送结束)

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART_RX_STA=0;       //接收状态标记
#endif

// 串口配置
void USART1_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;

	// 时钟配置
	USART1_RCC_Configuration();
	
	// 串口引脚配置
    USART1_GPIO_Configuration();

    //USART_InitStructure.USART_BaudRate = 921600;										// 波特率 mod 2400000-->921600
  USART_InitStructure.USART_BaudRate = 115200;  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							// 数据位
    //USART_WordLength_8b
    //USART_WordLength_9b
    USART_InitStructure.USART_StopBits = USART_StopBits_1;								// 停止位
    //USART_StopBits_1
    //USART_StopBits_0_5
    //USART_StopBits_2
    //USART_StopBits_1_5
    USART_InitStructure.USART_Parity = USART_Parity_No;									// 奇偶校验位
    //USART_Parity_No
    //USART_Parity_Even										// 偶校验
    //USART_Parity_Odd	   									// 奇校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		// 硬件流控制
    //USART_HardwareFlowControl_None
    //USART_HardwareFlowControl_RTS
    //USART_HardwareFlowControl_CTS
    //USART_HardwareFlowControl_RTS_CTS
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;						// 模式
    //USART_Mode_Tx
    //|USART_Mode_Rx
    USART_Init(USART3,&USART_InitStructure);
		//USART_Init(USART1,&USART_InitStructure);	sts new change
	
		//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3,ENABLE);
		//USART_Cmd(USART1,ENABLE);

	// USART DMA 使能
	 //USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	//USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	
	// 中断配置
    USART1_NVIC_Configuration();
}

// 串口引脚配置
void USART1_GPIO_Configuration(void)
	{																										//串口: pin9T pin10R
																										//USB: pin11M pin12P
    // 串口引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//sts new change
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   					// 复用推挽输出
    //GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_Init(GPIOA, &GPIO_InitStructure);
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //sts new change
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//sts new change
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   			// PU
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入   			
    //GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_Init(GPIOA, &GPIO_InitStructure);	//sts new change
}

// 串口时钟配置
void USART1_RCC_Configuration(void)
{
    // 时钟(I/O & USART1)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB1Periph_USART3,ENABLE);		//sts new change
	
}

//中断配置
void USART1_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		//NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // 串口中断配置
    USART_ITConfig(	USART3,	//USART_IT_PE		// 奇偶校验中断
                    //USART_IT_TXE		// 发送中断
                    //USART_IT_TC		// 传输完成中断
                    USART_IT_RXNE		// 接收中断
                    //|USART_IT_IDLE	// 空闲总线中断
                    //|USART_IT_LBD		// LIN中断检测中断
                    //|USART_IT_CTS		// CTS中断
                    //|USART_IT_ERR		// 错误中断
                    ,ENABLE);
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 			//bit31表明是否接收到0x0a(\n)
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;						//bit30表明是否接收到0x0d(\r)
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
   } 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
} 
