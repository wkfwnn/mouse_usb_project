#include "stm32_usart.h"


FlagStatus USART_SendEnd = RESET;	// �жϷ��ͽ�����־(SET:δ���ͽ���)

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART_RX_STA=0;       //����״̬���
#endif

// ��������
void USART1_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;

	// ʱ������
	USART1_RCC_Configuration();
	
	// ������������
    USART1_GPIO_Configuration();

    //USART_InitStructure.USART_BaudRate = 921600;										// ������ mod 2400000-->921600
  USART_InitStructure.USART_BaudRate = 115200;  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							// ����λ
    //USART_WordLength_8b
    //USART_WordLength_9b
    USART_InitStructure.USART_StopBits = USART_StopBits_1;								// ֹͣλ
    //USART_StopBits_1
    //USART_StopBits_0_5
    //USART_StopBits_2
    //USART_StopBits_1_5
    USART_InitStructure.USART_Parity = USART_Parity_No;									// ��żУ��λ
    //USART_Parity_No
    //USART_Parity_Even										// żУ��
    //USART_Parity_Odd	   									// ��У��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		// Ӳ��������
    //USART_HardwareFlowControl_None
    //USART_HardwareFlowControl_RTS
    //USART_HardwareFlowControl_CTS
    //USART_HardwareFlowControl_RTS_CTS
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;						// ģʽ
    //USART_Mode_Tx
    //|USART_Mode_Rx
    USART_Init(USART3,&USART_InitStructure);
		//USART_Init(USART1,&USART_InitStructure);	sts new change
	
		//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART3,ENABLE);
		//USART_Cmd(USART1,ENABLE);

	// USART DMA ʹ��
	 //USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	//USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	
	// �ж�����
    USART1_NVIC_Configuration();
}

// ������������
void USART1_GPIO_Configuration(void)
	{																										//����: pin9T pin10R
																										//USB: pin11M pin12P
    // ��������
    GPIO_InitTypeDef GPIO_InitStructure;
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//sts new change
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   					// �����������
    //GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_Init(GPIOA, &GPIO_InitStructure);
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //sts new change
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//sts new change
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   			// PU
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������   			
    //GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_Init(GPIOA, &GPIO_InitStructure);	//sts new change
}

// ����ʱ������
void USART1_RCC_Configuration(void)
{
    // ʱ��(I/O & USART1)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB1Periph_USART3,ENABLE);		//sts new change
	
}

//�ж�����
void USART1_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		//NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // �����ж�����
    USART_ITConfig(	USART3,	//USART_IT_PE		// ��żУ���ж�
                    //USART_IT_TXE		// �����ж�
                    //USART_IT_TC		// ��������ж�
                    USART_IT_RXNE		// �����ж�
                    //|USART_IT_IDLE	// ���������ж�
                    //|USART_IT_LBD		// LIN�жϼ���ж�
                    //|USART_IT_CTS		// CTS�ж�
                    //|USART_IT_ERR		// �����ж�
                    ,ENABLE);
}


void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 			//bit31�����Ƿ���յ�0x0a(\n)
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;						//bit30�����Ƿ���յ�0x0d(\r)
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
   } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 
